#pragma once
#include "result.hpp"

#include <vector>
#include <string>

#include <iostream>
#include <fstream>

namespace master_thesis
{
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
