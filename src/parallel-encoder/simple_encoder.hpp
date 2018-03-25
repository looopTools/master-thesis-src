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


#includer "thread_pool/ThreadPool.h"

////////////////////////////////////////////////////////////////////////////////////
// Begin Steinwurf INCLUDES                                                       //
////////////////////////////////////////////////////////////////////////////////////
#include <fifi/api/field.hpp>
#include <storage/storage.hpp>
#include <kodo_rlnc/full_vector_codes.hpp>

////////////////////////////////////////////////////////////////////////////////////
// Begin STL INCLUDES                                                             //
////////////////////////////////////////////////////////////////////////////////////
#include <cstdint>
#include <vector>
#include <mutex>


namespace master_thesis
{
using rlnc_encoder = kodo_rlnc::full_vector;

template<typename Data>
class simple_encoder
{

public:

    simple_encoder(uint32_t symbols, uint32_t symbol_size,
                   fifi::api::field field, Data data)
        : m_symbols(symbols), m_symbol_size(symbol_size),
          m_field(field), m_data(data), m_completed(0)
    {

    }

    void start()
    {

        // Create the ThreadPool with g threads, one for each generation size
        m_pool = ThreadPool(m_symbols);

        auto factory = rlnc_encoder::factory(m_field, m_symbols, m_symbol_size);
        auto encoder = factory.build();
        for (uint32_t i = 0; i < symbols; ++i)
        {
            // Enqueue the task in the ThreadPool
            // & Allows us to change the captured value in the lambda expression
            pool.enqueue([factory, encoder, m_data,
                          &m_mutex, &m_result, &m_completed](){
                             encoder->set_const_symbols(storage::storage(m_data));
                             Data payload(encoder->payload_size());
                             encoder->write_payload(payload.data());
                             // Lock so we can safely write to the result
                             m_mutex.lock();
                             m_result.pushback(payload);
                             ++m_completed;
                             m_mutex.unlock();
                         });
        }
    }

    bool completed()
    {
        return !(m_completed < (m_symbols - 1));
    }

    std::vector<Data> coded_symbols()
    {
        return m_results;
    }

private:

    uint32_t m_symbols;
    uint32_t m_symbol_size;
    uint32_t m_completed;
    fifi::api::field m_field;
    ThreadPool m_pool;
    Data m_data;
    std::vector<Data> m_result;
    std::mutex m_mutes;
};
}
