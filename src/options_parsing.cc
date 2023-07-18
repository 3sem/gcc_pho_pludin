#include "options_parsing.hh"

namespace gcc_reorder
{

int ShufflerOptionParser::parse_command_line_options(int argcount, char* arg_vec[])
    {
        char newFlag = 0;
        while((newFlag = getopt_long(argcount, arg_vec, short_opts.c_str(), longopts.data(), NULL)) != -1)
        {
            switch(newFlag)
            {
                case 'i':
                    input_file = optarg;
                    break;
                case 'o':
                    output_file = optarg;
                    break;
                case 'r':
                    restrict_file = optarg;
                    break;
                case 'l':
                    log_file = optarg;
                    break;
                case 'b':
                    breakdown_list2 = true;
                    break;
                case '?':
                default :
                    std::cerr << "Encountered unknown option" << std::endl;
                    break;
            }
        }

        return 0;
    }

} // namespace gcc_reorder