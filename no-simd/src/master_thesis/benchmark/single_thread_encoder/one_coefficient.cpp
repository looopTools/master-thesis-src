#include "../../result.hpp"
#include "../../write_result.hpp"
#include "../benchmark.hpp"
#include "../../config_reader.hpp"

#include <storage/storage.hpp>
#include <kodo_rlnc/full_vector_codes.hpp>

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
class one_coefficient_benchmark : public benchmark
{
    using rlnc_encoder = kodo_rlnc::shallow_full_vector_encoder<fifi::binary8>;
public:
    one_coefficient_benchmark(uint32_t itterations, config conf) :
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
        uint32_t threads = m_conf.threads();


        std::vector<uint8_t> data_in(symbols * symbol_size);

        // We are radomly filling the symbol matrix with data
        std::generate(data_in.begin(), data_in.end(), rand);

        rlnc_encoder::factory encoder_factory(symbols, symbol_size);

        auto encoder = encoder_factory.build();
        encoder->set_const_symbols(storage::storage(data_in));
        encoder->set_systematic_off();

        std::vector<uint8_t> payload(encoder->payload_size());
        auto start = std::chrono::high_resolution_clock::now();
        encoder->write_payload(payload.data());
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

    auto benchmark = master_thesis::benchmark::encoder::one_coefficient_benchmark(1000, conf);
    auto results = benchmark.run();

    master_thesis::write_result(master_thesis::generate_path("one_coefficient_encoder",
                                                             "benchmark",
                                                             conf), results);

    return 0;
}
