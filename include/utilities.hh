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

inline bool operator!=(const properties& lhs, const properties& rhs)
{
    return !(lhs == rhs);
}

struct pass_info
{
    std::string name;
    properties prop;
};

inline bool operator==(const pass_info& lhs, const pass_info& rhs)
{
    return (lhs.name == rhs.name) && (lhs.prop == rhs.prop);
}

inline bool operator!=(const pass_info& lhs, const pass_info& rhs)
{
    return !(lhs == rhs);
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
            std::size_t h2 = info.prop.destroyed << 1;
            std::size_t h3 = info.prop.provided << 2;
            std::size_t h4 = info.prop.required << 3;
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

    auto line_start = constraint_buf.begin();
    auto line_end = line_start;
    unsigned long add_starting_state = 0;

    for (unsigned long requirement = 1 << 18; (line_start != constraint_buf.end()) && (line_end != constraint_buf.end()); line_start++)
    {
        if ((*line_start) == '#')
        {
            line_start = std::find(line_start, constraint_buf.end(), '\n');
            continue;
        }
        if (((*line_start) == '\n') || ((*line_start) == ' '))
            continue;

        line_end = std::find(line_start, constraint_buf.end(), '\n');
        std::vector<std::string> line_of_passes = get_passes_seq(line_start, line_end);

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
        line_start = line_end;
    }

    return add_starting_state;
}

template <typename iter>
std::unordered_set<unsigned long> get_unique_requirements(iter begin, iter end)
{
    std::unordered_set<unsigned long> unique_requirements;
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