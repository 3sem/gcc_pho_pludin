#ifndef DRIVER_HH
#define DRIVE_HH

#include "state_machine.hh"
#include "file_parsing.hh"


template <typename LogParser, typename PassParser>
class Driver
{
    std::string descript_file_;
    std::vector<std::string> shuffled;
    PassListGenerator gen;
    LogParser log_parser;
    PassParser pass_parser;

    bool shuffle_only_one = false;

    bool breakdown_list2 = true;

public:
    Driver(const std::string& descript_file) : descript_file_(descript_file)
    {}

    void set_if_shuffle_multiple ( bool flag ) { shuffle_only_one = flag; }
    void set_if_breakdown_list2  ( bool flag ) { breakdown_list2 = flag; }

    std::pair<unsigned long, unsigned long> fill_gen_info_vec(const std::string& constraints_file)
    {
        log_parser.parse_log(descript_file_);
        auto&& [custom_start_state, custom_end_state] = log_parser.parse_constraints(constraints_file);
        // for (auto&& it : info_vec)
        //     std::cout << it.name << ' ' << it.prop.original.required << ' ' << it.prop.original.provided << ' ' << it.prop.original.destroyed <<
        //     ' ' << it.prop.custom.required << ' ' << it.prop.custom.provided << ' ' << it.prop.custom.destroyed << std::endl;
        // std::cout << "Starting & ending: " << custom_start_state << ' ' << custom_end_state << std::endl;
        gen.set_info_vec(log_parser.begin(), log_parser.end());

        return {custom_start_state, custom_end_state};
    }

    void fill_gen_pass_vec(const std::string& to_shuffle_file)
    {
        pass_parser.parse_passes_file(to_shuffle_file);
        if (to_shuffle_file == "lists/to_shuffle3.txt" && (std::find(pass_parser.begin(), pass_parser.end(), "loop2") == pass_parser.end()))
        {
            std::vector<std::string> passes = {pass_parser.begin(), pass_parser.end()};
            passes.push_back("loop2");
            gen.set_passes_vec(passes.begin(), passes.end());
        }

        gen.set_passes_vec(pass_parser.begin(), pass_parser.end());
    }

    int get_shuffled_vec(const std::string& to_shuffle_file, unsigned long starting_prop)
    {
        auto&& list_num = std::find_if(to_shuffle_file.begin(), to_shuffle_file.end(), [](const char c){ return isdigit(c); });
        std::string constraints_file = std::string{"lists/constraints"} + std::string{list_num, to_shuffle_file.end()};

        auto&& [custom_start_state, custom_end_state] = fill_gen_info_vec(constraints_file);
        fill_gen_pass_vec(to_shuffle_file);

        // for (auto&& it : gen.info_vec_)
        //     std::cout << it.name << ' ' << it.prop.original.required << ' ' << it.prop.original.provided << ' ' << it.prop.original.destroyed <<
        //     ' ' << it.prop.custom.required << ' ' << it.prop.custom.provided << ' ' << it.prop.custom.destroyed << std::endl;

        // for (auto&& it : gen.pass_vec_)
        //     std::cout << it << std::endl;

        gen.get_pass_name_to_id_maps();

        int failed = gen.shuffle_pass_order({starting_prop, custom_start_state}, {0, custom_end_state});
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
                pass_parser.parse_passes_file("lists/to_shuffle4.txt");
                assembled_list2.insert(++it, pass_parser.begin(), pass_parser.end());
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

    int generate_file_with_shuffle(const std::string& to_shuffle_file, const std::string& to_print_to)
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

        PassDumper to_dump(to_print_to);
        to_dump.dump(shuffled.begin(), shuffled.end());

        return 0;
    }
};

#endif