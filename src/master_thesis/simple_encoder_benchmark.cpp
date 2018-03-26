
#include "encoder/simple_encoder.hpp"

#include <kodo_rlnc/full_vector_codes.hpp>


#include <cstdint>
#include <vector>

int main()
{

    fifi::api::field field = fifi::api::field::binary8;

    uint32_t symbols = 10;
    uint32_t symbol_size = 40;

    std::vector<uint8_t> data;
    data.push_back(0u);
    master_thesis::simple_encoder encoder(symbols, symbol_size, field, data);
    // In the following we will make an encoder/decoder factory.
    // The factories are used to build actual encoders/decoders

    return 0;
}
