#include "../../result.hpp"
#include "../benchmark.hpp"
#include "../../encoder/smart_encoder.hpp"


#include <storage/storage.hpp>
#include <fifi/api/field.hpp>


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

        fifi::api::field field = fifi::api::field::binary8;

        std::vector<uint8_t> data_in(symbols * symbol_size);

        // We are radomly filling the symbol matrix with data
        std::generate(data_in.begin(), data_in.end(), rand);

        master_thesis::encoder::smart_encoder encoder(symbols, symbol_size, field, data_in);
        encoder.setup();

        auto start = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now());
        encoder.start();
        while(!encoder.completed()){}
        auto end = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now());

        std::cout << "DONE" << std::endl;

        auto res = result(start, end, 8, symbols, symbol_size);
        return res;
    }

private:
    uint32_t m_itterations;
    config m_conf;
};
}
}
}
