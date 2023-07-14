#ifndef UTILITIES_HH
#define UTILITIES_HH

#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <fstream>
#include <string>
#include <sstream>
#include <utility>
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
    properties original; // the once dumped with the help of plugin from gcc
    properties custom;   // custom ones, needed to restrict reordering if some hidden dependencies are found
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

namespace std // necessary specializations of std::hash to use std::unordered_...<pass_info> and std::unordered_...<std::pair<ulong, ulong>>
{
    template<>
    struct hash<pass_info>
    {
        typedef pass_info argument_type;
        std::size_t operator()(const pass_info& info) const
        {
            std::size_t h1 = std::hash<std::string>{}(info.name);
            std::size_t h2 = std::hash<unsigned long>{}(info.prop.custom.required + info.prop.original.required) << 1;
            std::size_t h3 = std::hash<unsigned long>{}(info.prop.custom.required + info.prop.original.required) << 2;
            std::size_t h4 = std::hash<unsigned long>{}(info.prop.custom.required + info.prop.original.required) << 3;
            return h1 ^ h2 ^ h3 ^ h4;
        }
    };

    template<>
    struct hash<std::pair<unsigned long, unsigned long>>
    {
        typedef std::pair<unsigned long, unsigned long> argument_type;
        std::size_t operator()(const argument_type& pair) const
        {
            std::size_t h1 = std::hash<unsigned long>{}(pair.first);
            std::size_t h2 = std::hash<unsigned long>{}(pair.second) << 1;
            return h1 ^ h2;
        }
    };
}


// function to read text from file into a std::string
// throws if cannot open a file
std::string get_file_text(const std::string& file_name);

// parses log (named unique_passes.txt) with information about passes' property restrictions from the gcc itself
std::vector<pass_info> parse_log(const std::string& info_file_name);

// gets passes from file to vector of strings
std::vector<std::string> parse_passes_file(const std::string& file_name);

// finds number, and return a pair of found number and iterator after position of number end
std::pair<unsigned long, std::string::const_iterator> find_number(std::string::const_iterator begin, const std::string& str);

// Deprecated
// Used to get passes in old format of custom property setting for passes
std::vector<std::string> get_passes_seq(std::string::const_iterator begin, std::string::const_iterator end);


template <typename iter>
unsigned long parse_constraints(iter begin, iter end, const std::string& constraint_file_name)
{
    std::string buf;
    try
    {
        buf = get_file_text(constraint_file_name);
    }
    catch(const std::ios_base::failure& exc)
    {
        std::cerr << "Could not open file " << constraint_file_name << " to get constraints info" << std::endl;
        return 0;
    }

    if (buf.empty())
        return 0;

    // skip initial comments, empty lines and spaces
    auto it = buf.cbegin();
    auto second_it = it;
    while((it = std::find(it, buf.cend(), '#')) != buf.cend())
    {
        second_it = std::find(it, buf.cend(), '\n');
        it = second_it;
    }
    it = ++second_it; // it after not finding another comment is at buf.cend, and second_it is at end of comment. So we se them both to next

    // get initiall custom property
    auto&& it_and_start_state = find_number(buf.cbegin(), buf);
    second_it = it = it_and_start_state.second;
    unsigned long add_starting_state = it_and_start_state.first;

    it++;
    for (; (it != buf.cend()) && (second_it != buf.cend()); it++)
    {
        // skip comments, empty lines and spaces
        if (*it == ' ' || *it == '\n')
            continue;
        if (*it == '#')
        {
            it = std::find(it, buf.cend(), '\n');
            second_it = it;
            continue;
        }

        pass_info info;

        // get pass name
        it = std::find_if(it, buf.cend(), [](const char c){ return isalpha(c);});
        second_it = std::find_if(it, buf.cend(), [](const char c){ return c == ' ';});

        info.name = std::string(it, second_it);

        // crutch, no idea why all passes have '_' or '-' as delimetres except for this one
        if (info.name == "rtl")
            info.name.append(" pre");

        // get properties
        auto&& req_iter_pair = find_number(second_it, buf);
        auto&& prov_iter_pair = find_number(req_iter_pair.second, buf);
        auto&& destr_iter_pair = find_number(prov_iter_pair.second, buf);

        // fill custom properties into existing pass_info structure about given pass
        auto&& to_fill_constraint_it = std::find_if(begin, end, [&name = info.name](const pass_info& info){ return name == info.name;});

        to_fill_constraint_it->prop.custom.required  |= req_iter_pair.first;
        to_fill_constraint_it->prop.custom.provided  |= prov_iter_pair.first;
        to_fill_constraint_it->prop.custom.destroyed |= destr_iter_pair.first;

        // std::cout << "Got: " << info.name << ' ' << req_iter_pair.first << ' ' << prov_iter_pair.first << ' ' << destr_iter_pair.first << std::endl;

        it = second_it = destr_iter_pair.second;
    }

    return add_starting_state;
}

// necessary for more efficient finding of passes, which original and custom required property are satisfied with the current state
template <typename iter>
std::unordered_set<std::pair<unsigned long, unsigned long>> get_unique_requirements(iter begin, iter end)
{
    std::unordered_set<std::pair<unsigned long, unsigned long>> unique_requirements;
    for (; begin != end; begin++)
    {
        unique_requirements.insert({begin->prop.original.required, begin->prop.custom.required});
    }

    return unique_requirements;
}

// Deprecated
// used for pass examination
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