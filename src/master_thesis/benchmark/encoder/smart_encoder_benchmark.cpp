#include "../../result.hpp"
#include "../../write_result.hpp"
#include "../benchmark.hpp"
#include "../../config_reader.hpp"
#include "../../encoder/smart_encoder.hpp"


#include <storage/storage.hpp>

#include <chrono>
#include <cstdint>
#include <vector>
#include <ratio>
#include <ctime>

#include <iostream>

namespace master_thesis
{
namespace benchmark
{
namespace encoder
{
class smart_encoder_benchmark : public benchmark
{
public:
    smart_encoder_benchmark(uint32_t itterations, config conf) :
        m_itterations(itterations), m_conf(conf){}

    std::vector<result> run()
    {
        for (uint32_t i = 0; i < m_itterations; ++i)
        {
            m_results.push_back(experiment());
        }
        return m_results;
    }

    result experiment()
    {
        // Seed the random number generator to get different random data
        srand(static_cast<uint32_t>(time(0)));

        uint32_t symbols = m_conf.symbols();
        uint32_t symbol_size = m_conf.symbol_size();
        auto threads = (uint32_t) std::thread::hardware_concurrency();

        std::vector<uint8_t> data_in(symbols * symbol_size);

        // We are radomly filling the symbol matrix with data
        std::generate(data_in.begin(), data_in.end(), rand);

        master_thesis::encoder::smart_encoder encoder(symbols, symbol_size, data_in);
        encoder.setup();

        auto start = std::chrono::high_resolution_clock::now();
        encoder.start();
        while(!encoder.completed()){}
        auto end = std::chrono::high_resolution_clock::now();

        auto c_start = std::chrono::duration_cast<std::chrono::microseconds>(start.time_since_epoch());
        auto c_end = std::chrono::duration_cast<std::chrono::microseconds>(end.time_since_epoch());
        auto res = result(c_start, c_end, threads, symbols, symbol_size);
        return res;
    }

private:
    uint32_t m_itterations;
    config m_conf;
};
}
}
}

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "file path must be provided" << std::endl;
        return -1;
    }

    std::string config_file = argv[1];
    auto conf = read_config(config_file);

    auto benchmark = master_thesis::benchmark::encoder::smart_encoder_benchmark(1000, conf);
    auto results = benchmark.run();

    master_thesis::write_result(master_thesis::generate_path("smart_encoder",
                                                             "benchmark",
                                                             conf), results);

    return 0;
}
