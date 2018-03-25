#include <kodo_rlnc/full_vector_codes.hpp>

int main()
{
    using rlnc_encoder = kodo_rlnc::full_vector_encoder;
    using rlnc_decoder = kodo_rlnc::full_vector_decoder;
    fifi::api::field field = fifi::api::field::binary8;

    // In the following we will make an encoder/decoder factory.
    // The factories are used to build actual encoders/decoders
    rlnc_encoder::factory encoder_factory(field, symbols, symbol_size);
    auto encoder = encoder_factory.build();

    return 0;
}
