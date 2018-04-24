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


////////////////////////////////////////////////////////////////////////////////////
// Begin Steinwurf INCLUDES                                                       //
////////////////////////////////////////////////////////////////////////////////////
#include <storage/storage.hpp>
#include <kodo_rlnc/full_vector_codes.hpp>


////////////////////////////////////////////////////////////////////////////////////
// Begin STL INCLUDES                                                             //
////////////////////////////////////////////////////////////////////////////////////
#include <cstdint>
#include <vector>
#include <mutex>

#include <memory>
namespace master_thesis
{
namespace encoder
{
class simple_encoder
{

    using rlnc_encoder = kodo_rlnc::shallow_full_vector_encoder<fifi::binary8>;

public:

    simple_encoder(uint32_t symbols, uint32_t symbol_size,
                   std::vector<uint8_t> data) :
        m_symbols(symbols), m_symbol_size(symbol_size),
        m_completed(0), m_data(data),  m_pool(symbols)
    {

    }


    // Have to be run - important!
    void setup()
    {

        auto t_data = storage::storage(m_data);
        rlnc_encoder::factory encoder_factory(m_symbols, m_symbol_size);
        for(uint32_t i = 0; i < m_symbols; ++i)
        {
            m_encoders.push_back(encoder_factory.build());
        }

        for(auto encoder : m_encoders)
        {
            encoder->set_const_symbols(t_data);
        }
    }

    void start()
    {
        for (uint32_t i = 0; i < m_symbols; ++i)
        {
            auto encoder = m_encoders.at(i);
            m_pool.enqueue([this, encoder](){
                               std::vector<uint8_t> payload(encoder->payload_size());
                               encoder->write_payload(payload.data());

                               this->m_mutex.lock();
                               this->m_result.push_back(payload);
                               ++(this->m_completed);
                               this->m_mutex.unlock();
                           });
        }
    }

    bool completed()
    {
        bool result = false;
        m_mutex.lock();
        result = !(m_completed < (m_symbols - 1));
        m_mutex.unlock();
        return result;
    }

    std::vector<std::vector<uint8_t>> result()
    {
        return m_result;
    }




private:

    uint32_t m_symbols;
    uint32_t m_symbol_size;
    uint32_t m_completed; // Used to see if encoder is done

    std::mutex m_mutex;

    std::vector<uint8_t> m_data;
    std::vector<std::vector<uint8_t>> m_result;

    ThreadPool m_pool;

    std::vector<std::shared_ptr<rlnc_encoder>> m_encoders;
};
}
}
