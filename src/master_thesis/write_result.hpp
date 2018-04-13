#pragma once
#include "result.hpp"
#include "config.hpp"

#include <vector>

#include <string>

#include <ctime>

#include <sstream>
#include <iostream>
#include <fstream>



namespace master_thesis
{

    std::string generate_path(std::string experiment_name, std::string type,
                              config& conf)
    {
        std::time_t result = std::time(nullptr);

        std::stringstream ss;
        ss << "./results/" << experiment_name << "/" << result << "_" << experiment_name
           << "_" << type << "_" << conf.threads() << "_" << conf.symbols()
           << "_" << conf.symbol_size();
        return ss.str();

    }

    bool write_result(std::string file_path, std::vector<result> results)
    {
        std::ofstream result_file;
        result_file.open(file_path);
        for (auto result : results)
        {
            result_file << result.to_string() << std::endl;
        }
        result_file.close();
        return true;
    }

}
