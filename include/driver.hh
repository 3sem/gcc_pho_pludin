#ifndef DRIVER_HH
#define DRIVE_HH

#include "state_machine.hh"



class Driver
{
    std::string descript_file_;
    std::vector<std::string> shuffled;
    PassListGenerator gen;

    bool shuffle_only_one = false;

    bool breakdown_list2 = true;

public:
    Driver(const std::string& descript_file) : descript_file_(descript_file)
    {}

    void set_if_shuffle_multiple ( bool flag ) { shuffle_only_one = flag; }
    void set_if_breakdown_list2  ( bool flag ) { breakdown_list2 = flag; }

    unsigned long fill_gen_info_vec(const std::string& constraints_file)
    {
        std::vector<pass_info> info_vec{parse_log(descript_file_)};
        unsigned long custom_start_state = parse_constraints(info_vec.begin(), info_vec.end(), constraints_file);
        gen.set_info_vec(info_vec.begin(), info_vec.end());

        // for (auto&& it : gen.info_vec_)
        //     std::cout << it.name << ' ' << it.prop.required << ' ' << it.prop.provided << ' ' << it.prop.destroyed << std::endl;

        return custom_start_state;
    }

    void fill_gen_pass_vec(const std::string& to_shuffle_file)
    {
        std::vector<std::string> passes{parse_passes_file(to_shuffle_file)};
        if (to_shuffle_file == "lists/to_shuffle3.txt" && (std::find(passes.begin(), passes.end(), "loop2") == passes.end()))
            passes.push_back("loop2");
        gen.set_passes_vec(passes.begin(), passes.end());
    }

    int get_shuffled_vec(const std::string& to_shuffle_file, unsigned long starting_prop)
    {
        auto&& list_num = std::find_if(to_shuffle_file.begin(), to_shuffle_file.end(), [](const char c){ return isdigit(c); });
        std::string constraints_file = std::string{"lists/constraints"} + std::string{list_num, to_shuffle_file.end()};
        unsigned long custom_start_state = fill_gen_info_vec(constraints_file);
        fill_gen_pass_vec(to_shuffle_file);
        gen.get_pass_name_to_id_maps();

        int failed = gen.shuffle_pass_order(custom_start_state | starting_prop);
        if (failed)
                return PassListGenerator::COULD_NOT_GEN;

        shuffled = {gen.begin(), gen.end()};
        return 0;
    }

    int generate_shuffled_list2()
    {
        static constexpr unsigned long FIRST_PART_START_PROP = 76079;
        static constexpr unsigned long SECOND_PART_START_PROP = FIRST_PART_START_PROP | (1 << 14) | (1 << 10) | (1 << 15);
        static constexpr unsigned long LOOP_START_PROP = SECOND_PART_START_PROP;

        std::string constraints_file_first_part = "lists/constraints2_1.txt";
        std::string constraints_file_second_part = "lists/constraints2_1.txt";

        int failed = 0;
        std::vector<std::string> assembled_list2;
        if (breakdown_list2)
        {
            std::string first_part_to_shuffle = std::string{"lists/to_shuffle2_1.txt"};
            failed = get_shuffled_vec(first_part_to_shuffle, FIRST_PART_START_PROP);
            if (failed)
                return PassListGenerator::COULD_NOT_GEN;

            assembled_list2 = std::move(shuffled);

            assembled_list2.push_back("pre");

            std::string second_part_to_shuffle = std::string{"lists/to_shuffle2_2.txt"};
            failed = get_shuffled_vec(second_part_to_shuffle, SECOND_PART_START_PROP);
            if (failed)
                return PassListGenerator::COULD_NOT_GEN;

            auto&& old_size = assembled_list2.size();
            assembled_list2.resize(assembled_list2.size() + shuffled.size());
            std::move(shuffled.begin(), shuffled.end(), assembled_list2.begin() + old_size);
        }
        else
        {
            failed = get_shuffled_vec("lists/to_shuffle2.txt", FIRST_PART_START_PROP);
            if (failed)
                return PassListGenerator::COULD_NOT_GEN;

            assembled_list2 = std::move(shuffled);
        }

        if (auto && it = std::find(assembled_list2.begin(), assembled_list2.end(), "loop"); (it != assembled_list2.end()))
        {
            if (shuffle_only_one)
            {
                std::vector<std::string> loop_passes{parse_passes_file("lists/to_shuffle4.txt")};
                assembled_list2.insert(++it, loop_passes.begin(), loop_passes.end());
            }
            else
            {
                std::string to_shuffle_loop = std::string{"lists/to_shuffle4.txt"};
                failed = get_shuffled_vec(to_shuffle_loop, LOOP_START_PROP);
                if (failed)
                    return PassListGenerator::COULD_NOT_GEN;

                for (auto&& str_it : shuffled)
                {
                    str_it.insert(str_it.begin(), '>');
                }
                shuffled.insert(shuffled.begin(), ">loopinit");
                shuffled.push_back(">loopdone");

                assembled_list2.insert(++it, shuffled.begin(), shuffled.end());
            }
        }

        shuffled = std::move(assembled_list2);

        return failed;
    }

    int generate_file_with_shuffle(const std::string& to_shuffle_file)
    {
        std::unordered_map<std::string, int> list_to_starting_property = { {"lists/to_shuffle1.txt", 76079}, {"lists/to_shuffle2.txt", 76079},
                                                                           {"lists/to_shuffle3.txt", 130760}, {"lists/to_shuffle4.txt", 126255}        };
        int failed = 0;
        if (to_shuffle_file == "lists/to_shuffle2.txt")
        {
            failed = generate_shuffled_list2();
            if (failed)
                return PassListGenerator::COULD_NOT_GEN;
        }
        else
        {
            failed = get_shuffled_vec(to_shuffle_file, list_to_starting_property[to_shuffle_file]);
            if (failed)
                return PassListGenerator::COULD_NOT_GEN;
        }

        auto&& list_num = std::find_if(to_shuffle_file.begin(), to_shuffle_file.end(), [](const char c){ return isdigit(c); });

        PassDumper to_dump(std::string{"list"} + std::string{list_num, to_shuffle_file.end()}, "", "");
        to_dump.dump(shuffled.begin(), shuffled.end());

        return 0;
    }
};

#endif