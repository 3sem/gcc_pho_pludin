#include "state_machine.hh"
#include "driver.hh"
#include <string_view>

int main(int argc, char* argv[])
{
    std::string to_shuffle_file = standart_pass_to_shuffle_file;
    bool breakdown_second_list = true;
    if (argc >= 2)
    {
        to_shuffle_file = argv[1];
        if (argc >= 3)
        {
            if (std::string_view{argv[2]} == std::string_view{"break"})
                breakdown_second_list = true;
            else if (std::string_view{argv[2]} == std::string_view{"nobreak"})
                breakdown_second_list = false;
        }
    }

    Driver driver(standart_pass_decription_file);

    if (to_shuffle_file == "lists/to_shuffle2.txt")
        driver.set_if_breakdown_list2(breakdown_second_list);
    driver.set_if_shuffle_multiple(false);

    return driver.generate_file_with_shuffle(to_shuffle_file);
}
