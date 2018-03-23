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

using rlnc_encoder = kodo_rlnc::full_vector_encoder;

class complex_encoder
{
public:
    template<typename Data>
    simple_encoder(uint32_t symbols, uint32_t symbol_size,
                   fifi::api::field field, Data data, uint32_t coefficients_per_thread) :
        m_symbols(symbols),
        m_symbol_size(symbol_size),
        m_field(field),
        m_data(data),
        m_factory(rlnc_encoder::factory(field, symbols, symbol_size)),
        m_completed(0),
        m_coefficients_per_thread(coefficients_per_thread)
        m_pool(ThreadPool::ThreadPool(symbols))
    {

    }

    void start()
    {

        uint32_t remainder = m_symbols % m_coefficients_per_thread;


        threads = (symbols - remainder) / m_coefficients_per_thread;

        for (uint32_t i = 0; i < threads; ++i)
        {
            pool.enqueue([](){
                    auto encoder = m_factory.build();
                    encoder->set_const_symbols(storage::storage(m_data));

                    for (uint32_t j = 0; j < m_coefficients_per_thread)
                    {
                        Data payload(encoder-payload_size());
                        encoder->write_payload(payload.data());
                        m_mutex.lock();
                        m_result.push_back(payload);
                        ++m_completed;
                        m_mutex.unlock();
                    }

                });
        }

        if (remainder != 0)
        {
            pool.enqueue([](){
                    auto encoder = m_factory.build();
                    encoder->set_const_symbols(storage::storage(m_data));

                    for (uint32_t j = 0; j < remainder)
                    {
                        Data payload(encoder-payload_size());
                        encoder->write_payload(payload.data());
                        m_mutex.lock();
                        m_result.push_back(payload);
                        ++m_completed;
                        m_mutex.unlock();
                    }

                });
        }


        uint32_t coefficients = 0;

        uint32_t remainder = 0;

        if (m_symbols % m_coefficients_per_thread = 0)
        {
            coefficients = m_symbols / m_coefficients_per_thread;
        }
        else
        {
            remainder = m_symbols % m_coefficients_per_thread;
            coefficients = (m_symbols - remainder) / m_coefficients_per_thread;
        }

        uint32_t end = 0;

        if (remainder != 0)
        {
            end =
        }

        for (uint32_t i = 0; i < m_symbols; ++i) {
            pool.enqueue([](){
                    auto encoder = m_factory.build();
                    encoder->set_const_symbols(storage::storage(m_data));
                    Data payload(encoder-payload_size());
                    encoder->write_payload(payload.data());
                    m_mutex.lock();
                    m_result.push_back(payload);
                    ++m_completed;
                    m_mutex.unlock();
                });
        }
    }

    bool completed()
    {
        return !(m_completed < m_symbols);
    }




private:
    uint32_t m_symbols;
    uint32_t m_symbol_size;
    fifi::api::field m_field;
    ThreadPool m_pool;
    Data m_data;
    std::vector<Data> m_result;
    rlnc_encoder::factory m_factory;
    std::mutex m_mutex;
    uint32_t m_completed;
    uint32_t m_coefficients_per_thread

};
}
