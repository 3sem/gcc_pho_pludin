#include "utilities.hh"

std::string get_file_text(const std::string& file_name)
{
    std::ifstream input;
    input.exceptions(std::ifstream::failbit);
    input.open(file_name);

    std::stringstream dump_buf;
    dump_buf << input.rdbuf();
    input.close();
    
    return dump_buf.str();
}

std::pair<unsigned long, std::string::const_iterator> find_number(std::string::const_iterator begin, const std::string& str)
{
    auto&& is_digit = [](const char c){return std::isdigit(c); };
    begin = std::find_if(begin, str.end(), is_digit);
    auto&& end = std::find_if_not(begin, str.end(), is_digit);

    return {std::stoi(std::string{begin, end}), end};
}

std::vector<std::string> get_passes_seq(std::string::const_iterator begin, std::string::const_iterator end)
{
    std::vector<std::string> pass_vec;
    auto it = begin;
    auto second_it = begin;

    for (; (it != end) && (second_it != end);)
    {
        second_it = std::find_if(it, end, [](const char c){ return c == ' ';});
        auto&& pass_name = std::string{it, second_it};
        if (pass_name == "rtl")
        {
            pass_name.append(" pre");
            second_it += 4; // len of " pre"
        }
        pass_vec.push_back(pass_name);

        it = std::find_if(second_it, end, [](const char c){ return std::isalpha(c) || (c == '!'); });
    }

    return pass_vec;
}

// TODO: seperate the common with parse_constraints part into another function
std::vector<pass_info> parse_log(const std::string& info_file_name)
{
    std::string buf;
    try
    {
        buf = get_file_text(info_file_name);
    }
    catch(const std::ios_base::failure& exc)
    {
        std::cerr << "Could not open file " << info_file_name << " to get passes info" << std::endl;
        throw;
    }

    std::vector<pass_info> info_vec;

    auto&& second_iter = buf.cbegin();
    for (auto&& iter = buf.cbegin(); (iter != buf.cend()) && (second_iter != buf.end()); iter++)
    {
        pass_info info;
        second_iter = std::find_if(iter, buf.cend(), [](const char c){ return c == ' ';});

        info.name = std::string(iter, second_iter);

        auto&& req_iter_pair = find_number(second_iter, buf);
        auto&& prov_iter_pair = find_number(req_iter_pair.second, buf);
        auto&& destr_iter_pair = find_number(prov_iter_pair.second, buf);

        info.prop = {{req_iter_pair.first, prov_iter_pair.first, destr_iter_pair.first}, {0, 0, 0}};

        iter = second_iter = destr_iter_pair.second;
        if (info.name == "rtl")
            continue;
        info_vec.push_back(info);
    }
    info_vec.push_back({"rtl pre", {{512, 0, 0}, {0, 0, 0}}});

    return info_vec;
}


std::vector<std::string> parse_passes_file(const std::string& file_name)
{
    std::string buf;
    try
    {
        buf = get_file_text(file_name);
    }
    catch(const std::ios_base::failure& exc)
    {
        std::cerr << "Could not open file " << file_name << " to get passes to reorder" << std::endl;
        throw;
    }

    std::vector<std::string> passes_vec;

    auto&& second_iter = buf.begin();
    for (auto&& iter = buf.begin(); (iter != buf.end()) && (second_iter != buf.end()); iter++)
    {
        second_iter = std::find_if(iter, buf.end(), [](const char c){ return c == '\n';});
        passes_vec.push_back({iter, second_iter});

        iter = second_iter;
    }

    return passes_vec;
}