#include "../config_reader.hpp"

#include <iostream>
int main(int argc, char* argv[])
{

    if (argc < 2)
    {
        std::cout << "file path must be provided" << std::endl;
        return -1;
    }

    std::string config_file = argv[1];
    auto conf = read_config(config_file);
    std::cout << conf.to_string() << std::endl;
}
