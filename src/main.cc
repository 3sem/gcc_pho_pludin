#include "state_machine.hh"
#include "driver.hh"

int main(int argc, char* argv[])
{
    std::string to_shuffle_file = standart_pass_to_shuffle_file;
    if (argc >= 2)
    {
        to_shuffle_file = argv[1];
    }

    auto&& list_num = std::find_if(to_shuffle_file.begin(), to_shuffle_file.end(), [](const char c){ return isdigit(c); });
    std::string constraints_file = std::string{"lists/constraints"} + std::string{list_num, to_shuffle_file.end()};

    Driver driver(standart_pass_decription_file, constraints_file);
    return driver.generate_file_with_shuffle(to_shuffle_file);
}
