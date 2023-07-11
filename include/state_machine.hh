#include <iostream>
#include <algorithm>
#include <numeric>
#include <random>
#include "utilities.hh"

const char standart_pass_decription_file[] = "unique_passes.txt";
const char standart_pass_to_shuffle_file[] = "pass_log_test.txt";
const char standart_dump_file[] = "shuffled.txt";

// Property state machine
//
// Gets a vector of properties, which is expected to actually be an map: pass id -> it's properties
// Then, one by one applies passes and changes the property state correspondingly
// If a pass is met, which's required properties of IR are not met, reports a failure to std::cerr
struct PropertyStateMachine
{
    std::unordered_map<int, properties> num_to_prop_;
    std::vector<int> passes_;

    int property_state = 1; // in gcc dumps, the first pass already required an existing property 0x1

    PropertyStateMachine(const std::unordered_map<int, properties>& num_to_prop) :
    num_to_prop_(num_to_prop)
    {}

    int apply_pass(int pass)
    {
        passes_.push_back(pass);
        properties pass_prop = num_to_prop_[pass];

        if ((property_state & pass_prop.required) != pass_prop.required)
        {
            report_failure(pass, pass_prop);
            return -1;
        }

        property_state |= pass_prop.provided;
        property_state &= ~pass_prop.destroyed;

        return 0;
    }

    void report_failure(int pass_num, const properties& pass_prop)
    {
        std::cerr << "Pass # " << passes_.size() << " with id:" << pass_num << " application failed due to current state being: " << std::hex << property_state
        << " and this pass requires at least folowing properties: " << pass_prop.required << std::endl;
    }
};


// This class gets a range of passes' names and properties fields (required / provided / destroyed) and range of passes names to be shuffled
// It build a hash map : property state -> vector of avaible passes
// Then, using PropertyStateMachine, built earlier hash map generates random sequence of given earlier passes
struct PassListGenerator
{
    std::vector<pass_info> info_vec_;
    std::unordered_map<std::string, int> name_to_id_map_; // we give each pass an id and work with ids to avoid needless heap indirection
    std::unordered_map<int, properties> pass_to_properties_; // hash map: pass id -> it's properties

    std::unordered_map<int, std::vector<int>> unique_requirement_to_passes_;

    std::vector<std::string> pass_vec_; // vector of passes to shuffle
    std::unordered_map<int, std::vector<std::string>> id_to_pass_batch;
    std::unordered_map<int, std::string> id_to_name;

    std::vector<std::string> shuffled_passes;

    static constexpr int USED_PASS = -2;
    static constexpr int MAX_PASS_AMOUNT = 250;

    template <typename iter_info, typename iter_name>
    PassListGenerator(iter_info begin_info, iter_info end_info, iter_name begin_name, iter_name end_name) : 
        info_vec_{begin_info, end_info}, pass_vec_{begin_name, end_name}
    {
        get_pass_to_number_map();
    }

    template <typename iter>
    void get_pass_batch_to_id(iter begin, iter end, int id, const properties& prop, const std::string& batch_name)
    {
        id_to_pass_batch[id] = {begin, end};
        for (; begin != end; begin++)
            name_to_id_map_[*begin] = id;
        pass_to_properties_[id] = prop;
    }

    template <typename iter>
    void erase_repeating(iter begin, iter end)
    {
        for (; begin != end; begin++)
        {
            auto&& to_erase = std::find(pass_vec_.begin(), pass_vec_.end(), *begin);
            if (to_erase != pass_vec_.end())
                pass_vec_.erase(to_erase);
        }
    }

    // map passes' names onto ids
    void get_pass_to_number_map()
    {
        int i = 0;

        for (auto&& it = info_vec_.begin(); it != info_vec_.end(); it++, i++)
        {
            name_to_id_map_[it->name] = i;
            id_to_name[i] = it->name;
            pass_to_properties_[i] = it->prop;
        }

        std::vector<std::string> for_inline = {"*rebuild_cgraph_edges", "inline_param"};
        std::vector<std::string> for_sched4 = {"split4", "sched2"};
        std::vector<std::string> for_loopinit = {"fix_loops", "loopinit"};
        std::vector<std::string> for_noloop = {"fix_loops", "no_loop"};
        std::vector<std::string> for_loops = {"loop2", "loop2_init", "loop2_invariant", "loop2_unroll", "loop2_doloop", "loop2_done"};

        std::vector pass_batches_names_vec = {for_inline, for_sched4, for_loopinit, for_noloop, for_loops};
        std::vector<properties> pass_batches_prop_vec = { {8, 0, 0}, {0, 0, 0}, {8, 0, 0}, {8, 0, 0}, {0, 0, 2048} };
        int j = 0;
        for (auto&& it : pass_batches_names_vec)
        {
            get_pass_batch_to_id(it.begin(), it.end(), i, pass_batches_prop_vec[j], it[it.size() - 1]);
            erase_repeating(it.begin(), it.begin() + it.size() - 1);
            i++, j++;
        }
    }

    // create a hash map : property state -> vector of avaible passes (pool of passes that could be used is defined by given range of passes to shufle)
    void generate_prop_passes_map()
    {
        auto&& unique_requirements = get_unique_requirements(info_vec_.begin(), info_vec_.end());

        for (auto&& pass_it : pass_vec_)
        {
            for (auto&& requirement_it : unique_requirements)
            {
                int required = pass_to_properties_[name_to_id_map_[pass_it]].required;
                if (required == requirement_it)
                    unique_requirement_to_passes_[requirement_it].push_back({name_to_id_map_[pass_it]});
            }
        }
    }

    // the shuffling itself
    void shuffle_pass_order(int initial_property_state)
    {
        shuffled_passes.clear();
        generate_prop_passes_map();

        PropertyStateMachine state(pass_to_properties_);
        state.property_state = initial_property_state;

        std::random_device rd;
        std::mt19937 gen(rd());

        std::vector<int> passes_to_choose_from;
        passes_to_choose_from.reserve(MAX_PASS_AMOUNT);


        for (int i = 0; i < pass_vec_.size(); i++)
        {
            int property_state = state.property_state;
            for (auto&& it : get_unique_requirements(info_vec_.begin(), info_vec_.end()))
            {
                if (((property_state & it) == it) && !unique_requirement_to_passes_[it].empty())
                {
                    auto&& old_size = passes_to_choose_from.size();
                    passes_to_choose_from.resize(passes_to_choose_from.size() + unique_requirement_to_passes_[it].size());
                    std::copy(unique_requirement_to_passes_[it].begin(), unique_requirement_to_passes_[it].end(),
                              passes_to_choose_from.begin() + old_size);
                }
            }

            if (passes_to_choose_from.empty())
                break;

            std::uniform_int_distribution<> to_get_index(0, passes_to_choose_from.size() - 1);

            int position_of_chosen_pass = to_get_index(gen);
            int chosen_pass = passes_to_choose_from[position_of_chosen_pass];
            state.apply_pass(chosen_pass);

            passes_to_choose_from.clear();

            auto&& to_erase_used_pass_from = unique_requirement_to_passes_[pass_to_properties_[chosen_pass].required];
            to_erase_used_pass_from.erase(std::find(to_erase_used_pass_from.begin(), to_erase_used_pass_from.end(), chosen_pass));

        }

        for (auto&& iter : state.passes_)
        {
            auto&& it = id_to_pass_batch.find(iter);
            if (it != id_to_pass_batch.end())
            {
                for (auto&& pass_batch_iter : it->second)
                    shuffled_passes.push_back(pass_batch_iter);
            }
            else
            {
                shuffled_passes.push_back(id_to_name[iter]);
            }

        }

    }

    void verify(int initial_property_state, const std::string& file_name)
    {
        std::vector<std::string> passes{parse_passes_file(file_name)};

        PropertyStateMachine state(pass_to_properties_);
        state.property_state = initial_property_state;

        for (auto&& it : passes)
        {
            // std::cout << "checking " << it << ' ' << "with property stuff:: " << pass_to_properties_[name_to_id_map_[it]].required << " ";
            // std::cout << pass_to_properties_[name_to_id_map_[it]].provided << " " << pass_to_properties_[name_to_id_map_[it]].destroyed;
            // std::cout << " and the state is " << state.property_state << std::endl;
            state.apply_pass(name_to_id_map_[it]);
        }
    }

    std::vector<std::string>::iterator begin() { return shuffled_passes.begin(); }
    std::vector<std::string>::iterator end() { return shuffled_passes.end(); }

};

class PassDumper
{
    std::string file_name_;
    std::string prefix_;
    std::string suffix_;

    std::vector<std::string> sub_passes;
public:
    PassDumper(const std::string& file_name, const std::string& prefix, const std::string& suffix) :
                file_name_(file_name), prefix_(prefix), suffix_(suffix)
    {
        init_sub_passes();
    }

    void init_sub_passes()
    {
        sub_passes = {"loop2_init", "loop2_invariant", "loop2_unroll", "loop2_doloop", "loop2_done"};
    }

    template <typename iter>
    void dump(iter begin, iter end)
    {
        std::stringstream buffer;
        for (auto&& it = begin; it != end; it++)
        {
            if (std::find(sub_passes.begin(), sub_passes.end(), *it) != sub_passes.end())
                buffer << ">";

            buffer << prefix_ << *it << suffix_ << std::endl;
        }

        std::ofstream output;
        output.open(file_name_);
        output << buffer.str();
    }
};