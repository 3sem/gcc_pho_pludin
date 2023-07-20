#include "state_machine.hh"
#include "driver.hh"
#include "file_parsing.hh"
#include "options_parsing.hh"

int main(int argc, char* argv[])
{
    if (argc < 4)
    {
        std::cerr << "A file of info about, file with passes to reorder and file to print the reordered sequence to are needed" << std::endl;
        return -1;
    }

    gcc_reorder::ShufflerOptionParser opt_parser(argc, argv);
    gcc_reorder::Driver<gcc_reorder::PassLogParser, gcc_reorder::PassToReorderParser, gcc_reorder::PassListGenerator> driver(opt_parser.log_file);

    driver.set_if_breakdown_list2(opt_parser.breakdown_list2);
    driver.set_if_ok_not_all_used(opt_parser.ok_if_not_all_used);

    int result = 0;
    try
    {
        result = driver.generate_file_with_shuffle(opt_parser.input_file, opt_parser.output_file);
    }
    catch(const std::ios_base::failure& exc)
    {
        return -1;
    }

    return result;
}
