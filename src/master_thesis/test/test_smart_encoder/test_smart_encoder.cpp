#include "../encoder/smart_encoder.hpp"

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <ctime>
#include <iostream>
#include <vector>

#include <storage/storage.hpp>
#include <fifi/default_field.hpp>

#include <kodo_rlnc/full_vector_codes.hpp>

int main()
{

    srand(static_cast<uint32_t>(time(0)));

    uint32_t symbols = 16;

    uint32_t symbol_size = 1000000;
    assert((symbol_size % 32) == 0);

    fifi::api::field field = fifi::api::field::binary8;

    std::vector<uint8_t> data_in(symbols * symbol_size);
    std::vector<uint8_t> data_out(symbols * symbol_size);

    // Fill the input data with random data
    std::generate(data_in.begin(), data_in.end(), rand);

    std::copy(data_in.begin(), data_in.end(), data_out.begin());

    using rlnc_decoder = kodo_rlnc::full_vector_decoder;
    rlnc_decoder::factory decoder_factory(field, symbols, symbol_size);
    auto decoder = decoder_factory.build();

    auto encoder = master_thesis::smart_encoder(symbols, symbol_size,
                                                field, data_in);


    return 0
}
