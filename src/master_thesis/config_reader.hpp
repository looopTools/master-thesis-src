#include "config.hpp"

#include <vector>
#include <cstdint>
#include <string>
#include <iostream>
#include <fstream>

std::vector<std::string> split(const std::string& str, const std::string& delim)
{
    std::vector<std::string> tokens;
    size_t prev = 0, pos = 0;
    do
    {
        pos = str.find(delim, prev);
        if (pos == std::string::npos) pos = str.length();
        std::string token = str.substr(prev, pos-prev);
        if (!token.empty()) tokens.push_back(token);
        prev = pos + delim.length();
    }
    while (pos < str.length() && prev < str.length());
    return tokens;
}

config read_config(std::string file_path)
{
    std::ifstream file(file_path);
    auto conf = config();

    std::string line;
    while (std::getline(file, line))
    {
        auto parameters = split(line, ",");

        std::string::size_type sz;
        uint32_t threads = std::stoi(parameters[0], &sz);
        uint32_t symbols = std::stoi(parameters[1], &sz);
        uint32_t symbol_size = std::stoi(parameters[2], &sz);

        conf.set_parameters(threads, symbols, symbol_size);
    }

    return conf;
}
