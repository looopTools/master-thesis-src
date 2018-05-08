#include "../../encoder/threaded_encoder.hpp"

#include <storage/storage.hpp>

#include <kodo_rlnc/full_vector_codes.hpp>

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <ctime>
#include <iostream>
#include <string>
#include <vector>

#include <chrono>

void test_case(uint32_t symbols, uint32_t symbol_size)
{
    srand(static_cast<uint32_t>(time(0)));

    std::vector<uint8_t> data_in(symbols * symbol_size);
    std::vector<uint8_t> data_out(symbols * symbol_size);

    std::generate(data_in.begin(), data_in.end(), rand);

    std::copy(data_in.begin(), data_in.end(), data_out.begin());

    using rlnc_decoder = kodo_rlnc::full_vector_decoder<fifi::binary8>;
    rlnc_decoder::factory decoder_factory(symbols, symbol_size);
    auto decoder = decoder_factory.build();

    master_thesis::encoder::threaded_encoder encoder(symbols, symbol_size,
                                                     storage::storage(data_in));

    auto start = std::chrono::high_resolution_clock::now();
    encoder.encode();
    while (!encoder.completed()){std::cout << "encoding" << std::endl;}
    auto end = std::chrono::high_resolution_clock::now();

    auto c_start = std::chrono::duration_cast<std::chrono::microseconds>(start.time_since_epoch());
    auto c_end = std::chrono::duration_cast<std::chrono::microseconds>(end.time_since_epoch());

    auto res = (c_end.count() - c_start.count()) / 1000.0;
    std::cout << res << std::endl;


    std::cout << "start decoding" << std::endl;
    for (auto payload : encoder.result())
    {
        decoder->read_payload(payload.data());
    }

    if (std::equal(data_out.begin(), data_out.end(), data_in.begin()))
    {
        std::cout << "DATA decoded correctly" << std::endl;
    }
    else
    {
        std::cout << "Unexpected Error" << std::endl;
    }
}

int main()
{
    test_case(8,67108864);
    test_case(16,33554432);
    test_case(32,16777216);
    test_case(64,8388608);
    test_case(128,4194304);
    test_case(256,2097152);
    return 0;
}
