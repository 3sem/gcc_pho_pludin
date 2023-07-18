#include "state_machine.hh"
#include "driver.hh"
#include "file_parsing.hh"
#include "options_parsing.hh"

int main(int argc, char* argv[])
{
    if (argc < 4)
    {
        std::cerr << "A file of info about, file with passes to reorder and file to print the reordered sequence to are needed" << std::endl;
        return -2;
    }

    ShufflerOptionParser opt_parser(argc, argv);
    Driver<PassLogParser, PassToReorderParser, PassListGenerator> driver(opt_parser.log_file);

    driver.set_if_breakdown_list2(opt_parser.breakdown_list2);

    return driver.generate_file_with_shuffle(opt_parser.input_file, opt_parser.output_file);
}
