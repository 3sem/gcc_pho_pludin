#include "state_machine.hh"

int main(int argc, char* argv[])
{
    std::string descript_file = standart_pass_decription_file;
    std::string to_shuffle_file = standart_pass_to_shuffle_file;
    std::string to_dump_file = standart_dump_file;
    if (argc >= 2)
    {
        to_shuffle_file = argv[1];
        if (argc >= 3)
        {
            to_dump_file = argv[2];
        }
        if (argc >= 4)
        {
            descript_file = argv[3];
        }
    }

    std::vector<pass_info> info_vec{parse_log(descript_file)};
    std::vector<std::string> passes{parse_passes_file(to_shuffle_file)};

    PassListGenerator gen {info_vec.begin(), info_vec.end(), passes.begin(), passes.end()};
    gen.shuffle_pass_order(1);

    PassDumper to_dump(to_dump_file, "", "");
    to_dump.dump(gen.begin(), gen.end());
    // gen.verify(1, to_dump_file);

}
