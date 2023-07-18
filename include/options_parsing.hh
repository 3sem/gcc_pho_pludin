#include <iostream>
#include <vector>
#include <getopt.h>

class ShufflerOptionParser
{

    const std::string short_opts = "l:i:o:r:b";

    const std::vector<option> longopts =
    {
        {"input_file", required_argument, NULL, 'i'},
        {"output_file", required_argument, NULL, 'o'},
        {"restrict_file", required_argument, NULL, 'r'},
        {"log_file", required_argument, NULL, 'l'},
        {"break", no_argument, NULL, 'b'},
    };

public:
    bool breakdown_list2 = false;
    std::string input_file;
    std::string output_file;
    std::string restrict_file;
    std::string log_file;

    ShufflerOptionParser(int argc, char* argv[])
    {
        parse_command_line_options(argc, argv);
    }


    int parse_command_line_options(int argcount, char* arg_vec[]);

};