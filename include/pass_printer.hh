#ifndef PASS_PRINTER_HH
#define PASS_PRINTER_HH

#include <fstream>
#include <string>
#include <sstream>

namespace gcc_reorder
{

// change how sub passes will be printed, so that plugin would work correctly
template <typename iter>
void modify_subpasses(iter str_vec_begin, iter str_vec_end)
{
    for (; str_vec_begin != str_vec_end; str_vec_begin++)
    {
        str_vec_begin->insert(str_vec_begin->begin(), '>');
    }
}

template <typename iter>
void print_passes_to_file(iter begin, iter end, const std::string& file_name, const std::string& prefix = "", const std::string& suffix = "")
{
    std::stringstream buffer;
    for (auto&& it = begin; it != end; it++)
    {
        buffer << prefix << *it << suffix << std::endl;
    }

    std::ofstream output;
    output.open(file_name);
    output << buffer.str();
}

} // namespace gcc_reorder

#endif