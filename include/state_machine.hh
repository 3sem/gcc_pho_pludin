#ifndef STATE_MACHINE_HH
#define STATE_MACHINE_HH

#include <iostream>
#include <algorithm>
#include <numeric>
#include <random>
#include "utilities.hh"

const char standart_pass_decription_file[] = "unique_passes.txt";
const char standart_pass_to_shuffle_file[] = "dummy_file_name_if_not_given.txt";
const char standart_dump_file[] = "shuffled.txt";

// Property state machine
//
// Gets a map: pass id -> it's properties
// Then, one by one applies passes and changes the property state correspondingly
// If a pass is met, which required properties are not met, reports a failure to std::cerr
//
// Also, has static functions to compress two set of properties/pass_properties into one
struct PropertyStateMachine
{
    std::unordered_map<int, pass_prop> num_to_prop_;
    std::vector<int> passes_;

    unsigned long original_property_state = 1; // in gcc dumps, the first pass already required an existing property 0x1
    unsigned long custom_property_state = 0;

    PropertyStateMachine(const std::unordered_map<int, pass_prop>& num_to_prop) :
    num_to_prop_(num_to_prop)
    {}

    // Compresses two sets of properties into one equivalent
    // The result is written into starting
    static void compress_property(properties& starting, const properties& to_apply)
    {
        starting.required |= to_apply.required;

        starting.provided |= to_apply.provided;
        starting.provided &= ~to_apply.destroyed;
        starting.destroyed |= to_apply.destroyed;
    }

    // Compresses two sets of pass properties into one equivalent
    // The result is written into starting
    static void compress_pass_prop(pass_prop& starting, const pass_prop& to_apply)
    {
        compress_property(starting.original, to_apply.original);
        compress_property(starting.custom, to_apply.custom);
    }

    int apply_pass(int pass)
    {
        passes_.push_back(pass);
        pass_prop pass_prop = num_to_prop_.at(pass);

        if (((original_property_state & pass_prop.original.required) != pass_prop.original.required) ||
            ((custom_property_state & pass_prop.custom.required) != pass_prop.custom.required))
        {
            report_failure(pass, pass_prop);
            return -1;
        }

        custom_property_state |= pass_prop.custom.provided;
        custom_property_state &= ~pass_prop.custom.destroyed;

        original_property_state |= pass_prop.original.provided;
        original_property_state &= ~pass_prop.original.destroyed;

        return 0;
    }

    void report_failure(int pass_num, const pass_prop& pass_prop)
    {
        std::cerr << "Pass # " << passes_.size() << " with id:" << pass_num << " application failed due to current state being: " << std::hex << original_property_state
        << " and " << std::hex << custom_property_state << " and this pass requires at least folowing original and custom properties: " <<
        std::hex << pass_prop.original.required << ' ' << std::hex << pass_prop.custom.required << std::endl;
    }
};


// This class gets a range of passes' names and properties fields (required / provided / destroyed; original and custom) and range of passes names to be shuffled
// It then builds hash maps: unique property requirement -> all passes that require it
// Then, using PropertyStateMachine, and built earlier hash map it fills a vector of passes that could be taken as next in generated sequence
// and chooses one randomly from this vector
struct PassListGenerator
{
    std::vector<pass_info> info_vec_;
    std::unordered_map<std::string, int> name_to_id_map_; // we give each pass an id and work with ids to avoid needless heap indirection
    std::unordered_map<int, pass_prop> pass_to_properties_; // hash map: pass id -> it's properties

    std::unordered_map<std::pair<unsigned long, unsigned long>, std::vector<int>> unique_requirement_to_passes_;

    std::vector<std::string> pass_vec_; // vector of passes to shuffle
    std::unordered_map<int, std::vector<std::string>> id_to_pass_batch; // some passes have to be together in resulting sequence, so we combine them into batches
                                                                        // give them the same id
    std::unordered_map<int, std::string> id_to_name;

    std::vector<std::string> shuffled_passes; // resulting shuffled passes sequence

    bool fail_if_not_all_passes_used = true; // flag to determine whether to return COULD_NOT_GEN if could not use all passes in starting list 
                                             // while reordering

    static constexpr int COULD_NOT_GEN = -1;
    static constexpr int USED_PASS = -2;
    static constexpr int MAX_PASS_AMOUNT = 250;
    static constexpr int TRY_AMOUNT = 1e4;

    PassListGenerator() = default;

    template <typename iter_info, typename iter_name>
    PassListGenerator(iter_info begin_info, iter_info end_info, iter_name begin_name, iter_name end_name) : 
        info_vec_{begin_info, end_info}, pass_vec_{begin_name, end_name}
    {
        get_pass_name_to_id_maps();
    }

    void set_fail_gen_flag (bool flag) { fail_if_not_all_passes_used = flag; }

    template <typename iter>
    void set_info_vec(iter begin, iter end)
    {
        info_vec_ = {begin, end};
    }

    template <typename iter>
    void set_passes_vec(iter begin, iter end)
    {
        pass_vec_ = {begin, end};
    }

    // Give a batch of passes a single id
    template <typename iter>
    void get_pass_batch_to_id(iter begin, iter end, int id, const pass_prop& prop, const std::string& batch_name)
    {
        id_to_pass_batch[id] = {begin, end};
        for (; begin != end; begin++)
            name_to_id_map_[*begin] = id;
        pass_to_properties_[id] = prop;
    }

    // leave a single pass from each batch in passes to reorder vector
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

    // map passes' names onto ids and batches of passes onto ids
    void get_pass_name_to_id_maps();

    // necessary for more efficient finding of passes, which original and custom required property are satisfied with the current state
    template <typename iter>
    std::unordered_set<std::pair<unsigned long, unsigned long>> get_unique_requirements(iter begin, iter end)
    {
        std::unordered_set<std::pair<unsigned long, unsigned long>> unique_requirements;
        for (; begin != end; begin++)
        {
            unique_requirements.insert({begin->prop.original.required, begin->prop.custom.required});
        }

        return unique_requirements;
    }

    // create a hash map : unique requirement -> all passes (from given range to reorder) that require it
    void generate_prop_passes_map();

    // the shuffling itself
    int shuffle_pass_order(const std::pair<unsigned long, unsigned long>& initial_property_state,
                           const std::pair<unsigned long, unsigned long>& ending_property_state);

    // Verify a sequence from file_name
    template <typename iter>
    void verify(iter begin, iter end, std::pair<unsigned long, unsigned long> initial_property_state)
    {
        PropertyStateMachine state(pass_to_properties_);
        state.original_property_state = initial_property_state.first;
        state.custom_property_state = initial_property_state.second;

        for (; begin != end; begin++)
        {
            // std::cout << "checking " << it << ' ' << "with property stuff:: " << pass_to_properties_[name_to_id_map_[it]].required << " ";
            // std::cout << pass_to_properties_[name_to_id_map_[it]].provided << " " << pass_to_properties_[name_to_id_map_[it]].destroyed;
            // std::cout << " and the state is " << state.property_state << std::endl;
            state.apply_pass(name_to_id_map_[*begin]);
        }
        std::cerr << state.original_property_state << state.custom_property_state << std::endl;
    }

    using iterator = std::vector<std::string>::iterator;
    using const_iterator = std::vector<std::string>::const_iterator;

    iterator begin() { return shuffled_passes.begin(); }
    iterator end() { return shuffled_passes.end(); }
    const_iterator begin() const { return shuffled_passes.begin(); }
    const_iterator end() const { return shuffled_passes.end(); }
    const_iterator cbegin() { return shuffled_passes.cbegin(); }
    const_iterator cend() { return shuffled_passes.cend(); }

};

#endif