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

////////////////////////////////////////////////////////////////////////////////////
// Begin Steinwurf INCLUDES                                                       //
////////////////////////////////////////////////////////////////////////////////////

#include <storage/storage,hpp>
#include <kodo_rlnc/full_vector_codes.hpp>

////////////////////////////////////////////////////////////////////////////////////
// Begin STL INCLUDES                                                             //
////////////////////////////////////////////////////////////////////////////////////
#include <cstdint>

namespace parallel_encoder
{
    template<typename DataType, class Threadpool>
class block_encoder
{
public:

    block_encoder(uint32_t symbols, uint32_t symbol_size, DataType data,
                  Threadpool threadpool) : m_symbols(symbols),
                                           m_symbol_size(symbol_size),
                                           m_data(data)
    {

    }







private:

    uint32_t m_symbols;
    uint32_t m_symbol_size;
    DataType m_data;
    Threadpool m_threadpool

};
}
