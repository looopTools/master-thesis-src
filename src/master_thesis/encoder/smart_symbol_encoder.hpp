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

#include <cassert>

#include <cstdint>
#include <vector>
#include <queue>

#include <thread>
namespace master_thesis
{
namespace encoder
{
class smart_symbol_encoder
{
    using rlnc_encoder = kodo_rlnc::shallow_full_vector_encoder<fifi::binary8>;

public:

    // Symbol width must be a multiple of the cache word length
    // Symbol Size effictively becomes symbol width, but we want you to type
    //        it any ways for reasons
    smart_symbol_encoder(uint32_t symbols, uint32_t symbol_size,
                         uint32_t symbol_width, uint32_t cache_size,
                         const storage::const_storage& symbol_storage) :
        m_symbols(symbols), m_symbol_size(symbol_size),
        m_symbol_width(symbol_width), m_cache_size(cache_size),
        m_completed(0), m_active_threads(0);
    {

        // If not the symbol_size % symbol_width == 0
        // this approach won't work with out padding a weird stuff
        // and as this is proof-of-concept we set this limitation
//        assert(symbols % symbol_width == 0);
        m_fragments = symbols;

        m_threads = static_cast<uint32_t>(std::thread::hardware_concurrency());

        while ((symbol_size * symbol_width * m_threads) > cache_size)
        {
            --m_threads;
        }

        // If m_threads !> 1 then this makes no sense
        assert(m_threads > 1);

        m_pool(m_threads); // initialise the size of the thread pool


        rlnc_encoder::factory encoder_factory(symbols, symbol_width);

        for (uint32_t i = 0; i < m_threads; ++i)
        {
            auto encoder = encoder_factory.build();
            encoder->set_const_symbols(symbol_storage);
            // Set systematic off, such that the encoder works as expected
            encoder->set_systematic_off();
            m_encoders.push_back(encoder);
            encoder_index.push(i);
        }

        // We prealloacted the m_result vector, to decrease latency of encoding
        m_result = std::vector<std::vector<uint8_t>>(symbols,
                                                     std::vector<uint8_t>(symbol_size));
    }

    void encode() {
        for (uint32_t i = 0; i < m_symbols; ++i)
        {
            // Generate coefficient vector
            // We generate g coefficient vectors
            std::vector<uint8_t> coefficients((m_encoders.at(0))->coefficient_vector_size());
            (m_encoders.at(0))->generate(coefficients.data());

            for (uint32_t j = 0; j < m_symbols; ++j)
            {

                m_pool.enqueue([this, coefficients, i, j, &coded_symbol](){
                        while(this->encoder_index.empty()){}

                        // This might need a mutex lock
                        // moste likely
                        auto e_index = this->encoder_index.front();
                        this->encoder_index.pop();

                        // Ready to receive the coded symbol
                        auto coded_symbol = std::vector<uint8_t>(m_symbol_width);

                        (this->m_encoders.at(e_index))->write_symbol(
                            coded_symbol.data(), coefficients.data());

                        index = j * this->m_symbols;

                        for (auto part : coded_symbol) {
                            this->m_result.at(i)[index] = part;
                            ++index;
                        }

                        this->encoder_index.push(e_index);
                        ++(this->m_fragements);
                    });
            }
            (m_results.at(i)).push_back(std:move(coefficients));
        }
    }

    bool completed()
    {
        return m_completed >= m_fragements * m_symbols;
    }


private:

    uint32_t m_symbols;
    uint32_t m_symbol_size;
    uint32_t m_symbol_width;
    uint32_t m_cache_size;
    uint32_t m_threads;
    uint32_t m_fragements;
    uint32_t m_completed;
    uint32_t m_active_threads;

    std::queue<uint32_t> encoder_index;

    std::vector<std::vector<uint8_t>> m_result;

    std::vector<std::shared_ptr<rlnc_encoder>> m_encoders;

    std::mutex m_mutex;

//    bool m_completed;

    ThreadPool m_pool;


};
}
}
