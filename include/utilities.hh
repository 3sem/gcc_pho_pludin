#ifndef UTILITIES_HH
#define UTILITIES_HH

#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

struct properties
{
    unsigned long required = 0;
    unsigned long provided = 0;
    unsigned long destroyed = 0;

};

inline bool operator==(const properties& lhs, const properties& rhs)
{
    return (lhs.required == rhs.required) && 
           (lhs.provided == rhs.provided) && 
           (lhs.destroyed == rhs.destroyed);
}

struct pass_prop
{
    properties original;
    properties custom;
};

inline bool operator==(const pass_prop& lhs, const pass_prop& rhs)
{
    return (lhs.original == rhs.original) && (lhs.custom == rhs.custom);
}

struct pass_info
{
    std::string name;
    pass_prop prop;
};

inline bool operator==(const pass_info& lhs, const pass_info& rhs)
{
    return (lhs.name == rhs.name) && (lhs.prop == rhs.prop);
}

namespace std
{
    template<>
    struct hash<pass_info>
    {
        typedef pass_info argument_type;
        std::size_t operator()(const pass_info& info) const
        {
            std::size_t h1 = std::hash<std::string>{}(info.name);
            std::size_t h2 = std::hash<std::string>{}(info.prop.custom.required + info.prop.original.required) << 1;
            std::size_t h3 = std::hash<std::string>{}(info.prop.custom.required + info.prop.original.required) << 2;
            std::size_t h4 = std::hash<std::string>{}(info.prop.custom.required + info.prop.original.required) << 3;
            return h1 ^ h2 ^ h3 ^ h4;
        }
    };
}


std::string get_file_text(const std::string& file_name);
std::vector<pass_info> parse_log(const std::string& info_file_name);
std::vector<std::string> parse_passes_file(const std::string& file_name);
std::pair<unsigned long, std::string::const_iterator> find_number(std::string::const_iterator begin, const std::string& str);
std::vector<std::string> get_passes_seq(std::string::const_iterator begin, std::string::const_iterator end);


template <typename iter>
unsigned long parse_constraints(iter begin, iter end, const std::string& constraint_file_name)
{
    std::string constraint_buf;
    try
    {
        constraint_buf = get_file_text(constraint_file_name);
    }
    catch(const std::ios_base::failure& exc)
    {
        std::cerr << "Could not open file " << constraint_file_name << " to get constraints info" << std::endl;
        return 0;
    }

    auto constr_it = constraint_buf.begin();
    auto line_end = constr_it;
    unsigned long add_starting_state = 0;

    for (unsigned long requirement = 1 << 18; (constr_it != constraint_buf.end()) && (line_end != constraint_buf.end()); constr_it++)
    {
        line_end = std::find(constr_it, constraint_buf.end(), '\n');

        std::vector<std::string> line_of_passes = get_passes_seq(constr_it, line_end);


        auto line_iter = line_of_passes.begin();
        auto second_line_iter = ++line_of_passes.begin();
        for (; second_line_iter != line_of_passes.end(); line_iter++, second_line_iter++ )
        {
            auto first_pass = std::find_if(begin, end,
                                           [&name = *line_iter](const pass_info& elem){return elem.name == name;});
            if (*second_line_iter == "!")
            {
                second_line_iter++;
                line_iter++;
                first_pass->prop.destroyed |= requirement;
                add_starting_state |= requirement;
            }
            else
            {
                first_pass->prop.provided |= requirement;
            }
            auto second_pass = std::find_if(begin, end,
                                            [&name = *second_line_iter](const pass_info& elem){return elem.name == name;});

            second_pass->prop.required |= requirement;
            requirement = requirement << 1;
        }
        constr_it = line_end;
    }

    return add_starting_state;
}

template <typename iter>
std::unordered_set<long> get_unique_requirements(iter begin, iter end)
{
    std::unordered_set<long> unique_requirements;
    for (; begin != end; begin++)
    {
        unique_requirements.insert(begin->prop.required);
    }

    return unique_requirements;
}

template <typename iter>
std::unordered_set<std::string> get_providing_passes(iter begin, iter end)
{
    std::unordered_set<std::string> providing_passes;

    for (; begin != end; begin++)
    {
        if (begin->prop.provided != 0)
            providing_passes.insert(begin->name);
    }

    for (auto&& it : providing_passes)
        std::cout << it << " ";

    return providing_passes;
}


#endif