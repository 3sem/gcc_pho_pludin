#include "state_machine.hh"
#include "driver.hh"

int main(int argc, char* argv[])
{
    std::string to_shuffle_file = standart_pass_to_shuffle_file;
    if (argc >= 2)
    {
        to_shuffle_file = argv[1];
    }

    Driver driver(standart_pass_decription_file);
    driver.generate_file_with_shuffle(to_shuffle_file);
}
