#include "state_machine.hh"
#include "driver.hh"
#include <string_view>

int main(int argc, char* argv[])
{
    std::string to_shuffle_file = standart_pass_to_shuffle_file; // by default has a dummy name, to show to more explicitly show in cerr
                                                                 // that file to shuffle was not given
    std::string to_print_to_file;
    bool breakdown_second_list = true; // second list's found very important pass is pre; so it was asked to add a possibility to shuffle 
                                       // all passes before pre and after pre independantly
    if (argc >= 2)
    {
        to_shuffle_file = argv[1];
        if (argc >= 3)
        {
            to_print_to_file = argv[2];
        }
        else
        {
            std::cerr << "A file to print the reordered sequence to is needed" << std::endl;
            return -2;
        }

        if (argc >= 4)
        {
            if (std::string_view{argv[3]} == std::string_view{"break"})
                breakdown_second_list = true;
            else if (std::string_view{argv[3]} == std::string_view{"nobreak"})
                breakdown_second_list = false;
        }
    }
    else
    {
        std::cerr << "A file of passes to reorder and name of file to print the reordered sequence to are needed" << std::endl;
        return -2;
    }

    Driver driver(standart_pass_decription_file);

    if (to_shuffle_file == "lists/to_shuffle2.txt")
        driver.set_if_breakdown_list2(breakdown_second_list);

    return driver.generate_file_with_shuffle(to_shuffle_file, to_print_to_file);
}
