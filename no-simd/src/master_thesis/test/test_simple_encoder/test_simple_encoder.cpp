#include "../../encoder/simple_encoder.hpp"

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <ctime>
#include <iostream>
#include <vector>

#include <storage/storage.hpp>

#include <kodo_rlnc/full_vector_codes.hpp>

int main()
{

    srand(static_cast<uint32_t>(time(0)));

    uint32_t symbols = 16;

    uint32_t symbol_size = 1000000;
    assert((symbol_size % 32) == 0);

    std::vector<uint8_t> data_in(symbols * symbol_size);
    std::vector<uint8_t> data_out(symbols * symbol_size);

    // Fill the input data with random data
    std::generate(data_in.begin(), data_in.end(), rand);

    std::copy(data_in.begin(), data_in.end(), data_out.begin());

    using rlnc_decoder = kodo_rlnc::full_vector_decoder<fifi::binary8>;
    rlnc_decoder::factory decoder_factory(symbols, symbol_size);
    auto decoder = decoder_factory.build();

    master_thesis::encoder::simple_encoder encoder(symbols, symbol_size, data_in);



    encoder.setup();
    encoder.start();
    while (!encoder.completed()) {
        std::cout << "Still Encoding" << std::endl;
    }

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

    return 0;
}
