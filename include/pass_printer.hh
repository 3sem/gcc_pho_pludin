#ifndef PASS_PRINTER_HH
#define PASS_PRINTER_HH

#include <fstream>
#include <string>
#include <sstream>

template <typename iter>
void modify_subpasses(iter str_vec_begin, iter str_vec_end)
{
    for (; str_vec_begin != str_vec_end; str_vec_begin++)
    {
        str_vec_begin->insert(str_vec_begin->begin(), '>');
    }
}

class PassPrinter
{
    std::string file_name_;
    std::string prefix_;
    std::string suffix_;
public:
    PassPrinter(const std::string& file_name, const std::string& prefix = "", const std::string& suffix = "") :
                file_name_(file_name), prefix_(prefix), suffix_(suffix)
    {}

    template <typename iter>
    void print(iter begin, iter end)
    {
        std::stringstream buffer;
        for (auto&& it = begin; it != end; it++)
        {
            buffer << prefix_ << *it << suffix_ << std::endl;
        }

        std::ofstream output;
        output.open(file_name_);
        output << buffer.str();
    }
};


#endif