// #include "state_machine.hh"
// #include "driver.hh"
#include "utilities.hh"

const char standart_pass_decription_file[] = "unique_passes.txt";


int main(int argc, char* argv[])
{
    auto&& info_vec{parse_log(standart_pass_decription_file)};

    parse_constraints(info_vec.begin(), info_vec.end(), "lists/constraints.txt");

    for (auto&& it : info_vec)
    {
        std::cout << it.name << ' ' << it.prop.original.required << ' ' << it.prop.original.provided << ' ' << it.prop.original.destroyed 
        << ' ' << it.prop.custom.required << ' ' << it.prop.custom.provided << ' ' << it.prop.custom.destroyed << std::endl;
    }
}
