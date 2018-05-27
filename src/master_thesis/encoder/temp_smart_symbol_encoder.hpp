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
#include <atomic>
#include <thread>

#include <iostream>

namespace master_thesis
{
namespace encoder
{
class smart_symbol_encoder
{
    using rlnc_encoder = kodo_rlnc::shallow_full_vector_encoder<fifi::binary8>;

public:

    // g: symbols - generation size
    // k: symbol_size - symbol size
    // l: fragement_width - amount of bytes in fragement
    // t: threads -- Maximum amount of concurrent threads
    // cache_size: cache size in bytes
    smart_symbol_encoder(uint32_t symbols, uint32_t symbol_size, uint32_t cache_size,
                         std::vector<uint8_t> data) : m_cache_size(cache_size),
        m_encoder_index(0),
        m_pool(std::thread::hardware_concurrency())
    {

        auto threads = static_cast<uint32_t>(std::thread::hardware_concurrency());

        m_fragement_width = cache_size / (threads * symbols);

        m_symbol_size = data.size() / (symbols * m_fragement_width);

        // m_data = transpose(data, symbols, m_fragement_width);
        rlnc_encoder::factory encoder_factory(symbols, m_fragement_width);


        uint32_t data_index = 0;

        for(uint32_t i = 0; i < m_symbol_size; ++i)
        {
            auto encoder = encoder_factory.build();
            std::vector<uint8_t> c_data;

            for (uint32_t j = 0; j < symbols * m_fragement_width; ++j)
            {
                c_data.push_back(data.at(data_index));
                ++data_index;
            }

            encoder->set_const_symbols(storage::storage(c_data));
            encoder->set_systematic_off();
            m_encoders.push_back(encoder);
        }




        // Pre generating all coefficients
        auto encoder = m_encoders.at(0); // We only need to use the first encoder


        for (uint32_t i = 0; i < symbols; ++i)
        {
            std::vector<uint8_t> coefficients(encoder->coefficient_vector_size());
            // Generate a coefficent vector
            encoder->generate(coefficients.data());
            m_coefficients.push_back(coefficients);
        }

        m_result = std::vector<std::vector<uint8_t>>(symbols,
                                                     std::vector<uint8_t>(data.size() / symbols));

    }

    void encode()
    {
        for (uint32_t i = 0; i < m_symbol_size; ++i)
        {
            auto encoder_index = get_encoder_index();
            m_pool.enqueue([this, encoder_index]{

                    auto encoder = this->m_encoders.at(encoder_index);


                    for (uint32_t j = 0; j < this->m_symbols; ++j)
                    {
                        auto coded_fragement = std::vector<uint8_t>(this->m_fragement_width);
                        auto coefficients = this->m_coefficients.at(j);
                        encoder->write_symbol(coded_fragement.data(), coefficients.data());
                        uint32_t index = encoder_index * this->m_fragement_width;
                        for(auto part : coded_fragement)
                        {
                            this->m_result.at(j)[index] = part;
                            ++index;
                        }
                    }
                    ++(this->m_completed);
                });
        }
    }

    bool completed()
    {
        return m_completed == (m_encoders.size() - 1);
    }

private:

    uint32_t get_encoder_index()
    {
        return m_encoder_index++;
    }

    std::vector<uint8_t> transpose(std::vector<uint8_t> data, uint32_t g, uint32_t l)
    {
        assert(data.size() % g == 0);

        auto result = std::vector<uint8_t>(data.size());

        for(uint32_t daniel = 0; daniel < (data.size() / g); ++daniel)
        {
            for(uint32_t i = 0; i < g; ++i)
            {
                for(uint32_t j = 0; j < l; ++j)
                {
                    result[j+i*l+daniel*g*l] = data.at(j + (i * (data.size()/g)) + daniel  * l);
                }
            }
        }

        return result;

    }



private:

    uint32_t m_symbols;
    uint32_t m_symbol_size;
    uint32_t m_fragement_width;
    uint32_t m_cache_size;
    uint32_t m_encoder_index;
    std::atomic<uint32_t> m_completed;

    std::vector<std::vector<uint8_t>> m_coefficients;
    std::vector<std::vector<uint8_t>> m_result;

    std::vector<std::shared_ptr<rlnc_encoder>> m_encoders;

    std::vector<uint8_t> m_data;

    std::mutex m_mutes;
    ThreadPool m_pool;

};
}
}
