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

#pragma once

#include <endian/big_endian.hpp>

#include <fifi/binary32.hpp>
#include <fifi/vector_do_product.hpp>

#include <cstdint>
#include <cassert>
#include <algorithm>
#include <type_traits>
#include <vector>

namespace master_thesis
{
template<class SuperCoder>
class parallel_block_encoder
{
public:
    uint32_t write_payloads(uint8_t** payloads, uint32_t payload_count)
    {
        return 0U;
    }
};
}
