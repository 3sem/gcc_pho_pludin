#include "state_machine.hh"


// map passes' names onto ids and batches of passes onto ids
void PassListGenerator::get_pass_name_to_id_maps()
{
    int i = 0;

    for (auto&& it = info_vec_.begin(); it != info_vec_.end(); it++, i++)
    {
        name_to_id_map_[it->name] = i;
        id_to_name[i] = it->name;
        pass_to_properties_[i] = it->prop;
    }

    std::vector<std::string> for_sched4 = {"split4", "sched2"};
    std::vector<std::string> for_loopinit = {"fix_loops", "loop"};
    std::vector<std::string> for_noloop = {"fix_loops", "no_loop"};
    std::vector<std::string> for_loops = {"loop2", "loop2_init", "loop2_invariant", "loop2_unroll", "loop2_doloop", "loop2_done"};

    std::vector pass_batches_names_vec = {for_sched4, for_loopinit, for_noloop, for_loops};
    std::vector<pass_prop> pass_batches_prop_vec;
    for (int k = 0; k < pass_batches_names_vec.size(); k++)
    {
        pass_prop prop_for_batch;

        for (auto&& name_it : pass_batches_names_vec[k])
        {
            auto&& pass_info_it = std::find_if(info_vec_.begin(), info_vec_.end(), [&name = name_it](const pass_info& info){ return info.name == name; });

            PropertyStateMachine::compress_pass_prop(prop_for_batch, pass_info_it->prop);
        }

        // we place a combined pass property struct of whole batch into info_vec so that the function that generates unique requirements set would also
        // include the generated batches requirements
        info_vec_.push_back({std::string{"batch_"} + pass_batches_names_vec[k].back(), prop_for_batch});


        pass_batches_prop_vec.push_back(prop_for_batch);
    }

    int j = 0;
    for (auto&& it : pass_batches_names_vec)
    {
        get_pass_batch_to_id(it.begin(), it.end(), i, pass_batches_prop_vec[j], it[it.size() - 1]);
        erase_repeating(it.begin(), it.begin() + it.size() - 1);
        i++, j++;
    }
}

// create a hash map : unique requirement -> all passes (from given range to reorder) that require it
void PassListGenerator::generate_prop_passes_map()
{
    auto&& unique_requirements = get_unique_requirements(info_vec_.begin(), info_vec_.end());

    for (auto&& pass_it : pass_vec_)
    {
        for (auto&& requirement_it : unique_requirements)
        {
            auto&& orig_required = pass_to_properties_.at(name_to_id_map_[pass_it]).original.required;
            auto&& custom_required = pass_to_properties_.at(name_to_id_map_[pass_it]).custom.required;
            if (std::pair{orig_required, custom_required} == requirement_it)
                unique_requirement_to_passes_[requirement_it].push_back({name_to_id_map_[pass_it]});
        }
    }
}

// the shuffling itself
int PassListGenerator::shuffle_pass_order(const std::pair<unsigned long, unsigned long>& initial_property_state,
                                          const std::pair<unsigned long, unsigned long>& ending_property_state)
{
    PropertyStateMachine state(pass_to_properties_);

    // Sometimes due to properties restrictions in given range of passes some pass1 cannot be after pass2; and if pass1 is taken before pass2
    // pass2 wont be in resulting sequence at all
    // we try to avoid not using all passes in resulting sequence, so TRY_AMOUNT tries are made, so that all passes would be used
    //
    // There is a flag fail_if_not_all_passes_used, which determines, whether the generation o sequence fails alltogether, if all sequence with all
    // passes could be generated, or just the last sequence is left as resulting
    for (int i = 0; (i < TRY_AMOUNT) && (state.passes_.size() != pass_vec_.size()); i++)
    {
        // clear the previously generated sequence if there was one
        // and generate all necessary maps

        // std::cout << "TRY#" << i << std::endl;
        state.passes_.clear();
        unique_requirement_to_passes_.clear();
        shuffled_passes.clear();
        generate_prop_passes_map();

        state.original_property_state = initial_property_state.first;
        state.custom_property_state = initial_property_state.second;

        std::random_device rd;
        std::mt19937 gen(rd());

        std::vector<int> passes_to_choose_from;
        passes_to_choose_from.reserve(MAX_PASS_AMOUNT); // we reserve enough space, to avoid unnecessary reallocations

        for (int i = 0; i < pass_vec_.size(); i++)
        {
            auto&& property_state = std::pair{state.original_property_state, state.custom_property_state};

            // fill vector of available passes
            for (auto&& it : get_unique_requirements(info_vec_.begin(), info_vec_.end()))
            {
                // check if passes requirements are mets
                if (((property_state.first & it.first) == it.first) && ((property_state.second & it.second) == it.second) &&
                    !unique_requirement_to_passes_[it].empty())
                {
                    // copy all passes with given requirement into vector of passes to choose from
                    auto&& old_size = passes_to_choose_from.size();
                    passes_to_choose_from.resize(passes_to_choose_from.size() + unique_requirement_to_passes_[it].size());
                    std::copy(unique_requirement_to_passes_[it].begin(), unique_requirement_to_passes_[it].end(),
                            passes_to_choose_from.begin() + old_size);
                }
            }

            if (passes_to_choose_from.empty())
                break;

            // choose a pass randomly

            // std::cout << "Available: " << std::endl;
            // for (auto&& it : passes_to_choose_from)
            // {
            //     std::cout << id_to_name[it] << ' ';
            // }
            // std::cout << std::endl;

            std::uniform_int_distribution<> to_get_index(0, passes_to_choose_from.size() - 1);

            int position_of_chosen_pass = to_get_index(gen);
            int chosen_pass = passes_to_choose_from[position_of_chosen_pass];
            // std::cout << "Chosen: " << id_to_name[chosen_pass] << std::endl;

            // std::cout << "Before " << state.custom_property_state << std::endl;
            state.apply_pass(chosen_pass);
            // std::cout << "After " << state.custom_property_state << std::endl;


            // reset available passes and erase used pass from pool of all passes to reorder
            passes_to_choose_from.clear();
            auto&& properties_of_chosen = pass_to_properties_.at(chosen_pass);

            auto&& to_erase_used_pass_from = unique_requirement_to_passes_[{properties_of_chosen.original.required, properties_of_chosen.custom.required}];
            to_erase_used_pass_from.erase(std::find(to_erase_used_pass_from.begin(), to_erase_used_pass_from.end(), chosen_pass));
        }

        // std::cout << state.original_property_state << ' ' << state.custom_property_state << std::endl;

        // if could not meet required state by the end of sequence - regenerate
        if (((state.original_property_state & ending_property_state.first) != ending_property_state.first) ||
            ((state.custom_property_state & ending_property_state.second) != ending_property_state.second))
        {
            state.passes_.clear();
            continue;
        }

        // map from resulting sequence of passes ids back to names
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

    if ((state.passes_.size() != pass_vec_.size()) && fail_if_not_all_passes_used)
        return COULD_NOT_GEN; // if could not generate sequence with all passes and flag to fail in this scenario is set

    return 0;
}