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

#pragma once

#include "../threadpool/threadpool.h"

#include <storage/storage.hpp>
#include <kodo_rlnc/full_vector_codes.hpp>

#include <cstdint>

namespace master_thesis
{
namespace encoder
{
class smart_symbol_encoder
{
    using rlnc_encoder = kodo_rlnc::shallow_full_vector_encoder<fifi::binary8>;

public:

    smart_symbol_encoder(uint32_t symbols, uint32_t symbol_size,
                         uint32_t symbol_width, const storage::const_storage& symbol_storage)

    {}

};
}
}