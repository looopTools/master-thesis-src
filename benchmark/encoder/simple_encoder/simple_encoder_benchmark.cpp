#include <master_thesis/simple_encoder.hpp>

#include <vector>
#include <cstdint>
int main()
{
    std::vector<uint8_t> a;
    auto encoder =
        master_thesis::simple_encoder<std::vector<uint8_t>>(8, 9,
                                                            fifi::api::field::binary32);
    encoder.start();
    while(!encoder.completed())
}
