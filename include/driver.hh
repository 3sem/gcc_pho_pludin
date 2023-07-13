#ifndef DRIVER_HH
#define DRIVE_HH

#include "state_machine.hh"



class Driver
{
    std::string descript_file_;
    std::string constraint_file_;

    bool shuffle_only_one = false;

public:
    Driver(const std::string& descript_file, const std::string& constraint_file) : descript_file_(descript_file), constraint_file_(constraint_file)
    {}

    void set_if_shuffle_multiple ( bool flag ) { shuffle_only_one = flag; }

    int generate_file_with_shuffle(const std::string& to_shuffle_file)
    {
        std::vector<pass_info> info_vec{parse_log(descript_file_)};
        unsigned long custom_start_state = parse_constraints(info_vec.begin(), info_vec.end(), constraint_file_);
        std::vector<std::string> passes{parse_passes_file(to_shuffle_file)};

        // for (auto&& it : info_vec)
        //     std::cout << it.name << ' ' << it.prop.required << ' ' << it.prop.provided << ' ' << it.prop.destroyed << std::endl;

        std::unordered_map<std::string, int> list_to_starting_property = { {"lists/to_shuffle1.txt", 76079}, {"lists/to_shuffle2.txt", 76079},
                                                                           {"lists/to_shuffle3.txt", 130760}, {"lists/to_shuffle4.txt", 126255}        };

        if (to_shuffle_file == "lists/to_shuffle3.txt" && (std::find(passes.begin(), passes.end(), "loop2") == passes.end()))
            passes.push_back("loop2");

        PassListGenerator gen {info_vec.begin(), info_vec.end(), passes.begin(), passes.end()};
        bool failed = gen.shuffle_pass_order(custom_start_state | list_to_starting_property[to_shuffle_file]);

        if (failed)
            return PassListGenerator::COULD_NOT_GEN;

        std::vector<std::string> shuffled {gen.begin(), gen.end()};

        if (auto && it = std::find(shuffled.begin(), shuffled.end(), "loop"); it != shuffled.end())
        {
            std::vector<std::string> loop_passes{parse_passes_file("lists/to_shuffle4.txt")};
            if (!shuffle_only_one)
            {
                PassListGenerator gen_loop_list {info_vec.begin(), info_vec.end(), loop_passes.begin(), loop_passes.end()};
                gen_loop_list.shuffle_pass_order(custom_start_state | list_to_starting_property["lists/to_shuffle4.txt"]);

                // std::vector<std::string> shuffled_list4 {gen_loop_list.begin(), gen_loop_list.end()};
                // shuffled_list4.push_back("loopdone");

                for (auto&& str_it : gen_loop_list)
                    str_it.insert(str_it.begin(), '>');

                it = shuffled.insert(++it, ">loopdone");
                it = shuffled.insert(it, gen_loop_list.begin(), gen_loop_list.end());
                shuffled.insert(it, ">loopinit");
            }
            else
            {
                shuffled.insert(it, loop_passes.begin(), loop_passes.end());
            }
        }

        std::string to_dump_file;
        auto&& list_num = std::find_if(to_shuffle_file.begin(), to_shuffle_file.end(), [](const char c){ return isdigit(c); });

        PassDumper to_dump(std::string{"list"} + std::string{list_num, to_shuffle_file.end()}, "", "");
        to_dump.dump(shuffled.begin(), shuffled.end());

        return 0;
    }
};

#endif