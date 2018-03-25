#include "simple_encoder.hpp"

#include <vector>
#include <cstdint>

#include <fifi/api/field.hpp>
int main()
{
    std::vector<uint8_t> a(8*32);



    auto encoder =
        master_thesis::simple_encoder<std::vector<uint8_t>>(8, 32,
                                                            fifi::api::field::binary32, a);
    encoder.start();
    while(!encoder.completed())
}
