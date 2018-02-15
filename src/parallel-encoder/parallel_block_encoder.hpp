////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2017 Lars Nielsen                                                //
////////////////////////////////////////////////////////////////////////////////////
// Permission is hereby granted, free of charge, to any person obtaining a copy   //
// of this software and associated documentation files (the "Software"), to deal  //
// in the Software without restriction, including without limitation the rights   //
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      //
// copies of the Software, and to permit persons to whom the Software is          //
// furnished to do so, subject to the following conditions:                       //
////////////////////////////////////////////////////////////////////////////////////
// NOTICE:                                                                        //
// This encoder is designed such that it will be compatible with the Kodo         //
// RLNC software libary and therefore is the same termonology applied in the code.//
// This means that:                                                               //
//      symbols = generation size                                                 //
////////////////////////////////////////////////////////////////////////////////////


/// https://github.com/steinwurf/kodo-rlnc/blob/d43ebed4277be2a7106f7f84b5f47be3eb435133/examples/encode_decode_using_coefficients/encode_decode_using_coefficients.cpp
/// https://github.com/steinwurf/kodo-rlnc/blob/master/src/kodo_rlnc/full_vector_encoder.hpp

#pragma once

////////////////////////////////////////////////////////////////////////////////////
// Begin Steinwurf INCLUDES                                                       //
////////////////////////////////////////////////////////////////////////////////////
#include <kodo_core/api/encoder_interface.hpp>
#include <kodo_core/api/factory_interface.hpp>

////////////////////////////////////////////////////////////////////////////////////
// Begin STL INCLUDES                                                             //
////////////////////////////////////////////////////////////////////////////////////
#include <memory>
#include <cstdint>
// #include <endian/big_endian.hpp>

// #include <fifi/binary32.hpp>
// #include <fifi/vector_do_product.hpp>

// #include <cstdint>
// #include <cassert>
// #include <algorithm>
// #include <type_traits>
// #include <vector>

namespace master_thesis
{
template<class SuperCoder>
class parallel_block_encoder : public kodo_core::api::encoder_interface
{
public:
    // Follow the facotry pattern enabling the construction of
    // parallel encoder
    class factory : public kodo_core::api::factory_interface
    {
    public:
        using pointer = std::shared_ptr<parallel_block_encoder>;

        factory(fifi:api::field field, uint32_t symbols, uint32_t symbol_size);

        uint32_t symbols() const;

        uint32_t symbol_size() const;

        void set_symbols(uint32_t symbols);

        void set_symbol_size(uint32_t symbol_size);

        pointer build();

    private:
        friend class full_vector_encoder;

        struct impl;
        std::shared_ptr<impl> m_impl;
    };

    uint32_t symbols() const;

    uint32_t symbol_size() const;

    uint32_t block_size();


};
}
