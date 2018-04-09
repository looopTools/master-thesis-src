#include "result.hpp"
#include "../benchmark.hpp"
#include "../../encoder/smart_encoder.hpp"


#include <storage/storage.hpp>
#include <fifi/api/field.hpp>


#include <chrono>
#include <cstdint>
#include <vector>

#include <ctime>

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
        m_itterations, m_conf(conf){}

    std::vector<result> run()
    {
        for (uint32_t i; i < itterations; ++i)
        {
            result.push_back(experiment());
        }
    }

    result experiment()
    {
        // Seed the random number generator to get different random data
        srand(static_cast<uint32_t>(time(0)));

        uint32_t symbols = m_conf.symbols();
        uint32_t symbols_size = m_conf.symbol_size;

        fifi::api::field field = fifi::api::field::binary8;

        std::vector<uint8_t> data_in(symbols * symbols_size);

        // We are radomly filling the symbol matrix with data
        std::generate(data_in.begin(), data_in.end(). rand);
    }

private:
    uint32_t m_itterations;
    config m_conf;
};
}
}
}
