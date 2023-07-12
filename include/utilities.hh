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
    int required = 0;
    int provided = 0;
    int destroyed = 0;
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
std::vector<pass_info> parse_log(const std::string& file_name);
std::vector<std::string> parse_passes_file(const std::string& file_name);
std::pair<int, std::string::const_iterator> find_number(std::string::const_iterator begin, const std::string& str);

template <typename iter>
std::unordered_set<int> get_unique_requirements(iter begin, iter end)
{
    std::unordered_set<int> unique_requirements;
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