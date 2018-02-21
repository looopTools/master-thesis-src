// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>
#include <thread>
#include <sak/convert_endian.hpp>

#include "full_vector_codes.hpp"

namespace kodo
{
namespace rlnc
{
    template<uint32_t Threads, class Stack, class SuperCoder>
    class thread_encoder_impl : public SuperCoder
    {
    public:

        /// The factory layer associated with this coder.
        /// In this case only needed to provide the max_payload_size()
        /// function.
        class factory_base : public SuperCoder::factory_base
        {
        public:

            /// @copydoc layer::factory::factory(uint32_t,uint32_t)
            factory_base(uint32_t max_symbols, uint32_t max_symbol_size) :
                SuperCoder::factory_base(max_symbols, max_symbol_size),
                m_stack_factory(max_symbols, max_symbol_size / Threads)
            {
                assert(max_symbol_size > 0);
                assert((max_symbol_size % Threads) == 0);
            }

        protected:

            friend class thread_encoder_impl;

            typename Stack::factory& stack_factory()
            {
                return m_stack_factory;
            }

            typename Stack::factory m_stack_factory;
        };

    public:

        template<class Factory>
        void construct(Factory& the_factory)
        {
            SuperCoder::construct(the_factory);

            m_coefficients.resize(Threads);
            for(auto& c : m_coefficients)
            {
                c.resize(the_factory.max_coefficient_vector_size());
            }
        }

        template<class Factory>
        void initialize(Factory& the_factory)
        {
            SuperCoder::initialize(the_factory);

            m_stacks.clear();

            auto& stack_factory = the_factory.stack_factory();
            stack_factory.set_symbols(the_factory.symbols());

            uint32_t symbol_size = the_factory.symbol_size();

            assert((symbol_size % Threads) == 0);

            uint32_t thread_symbol_size = symbol_size / Threads;
            assert(thread_symbol_size * Threads == symbol_size);

            stack_factory.set_symbol_size(thread_symbol_size);

            for (uint32_t i = 0; i < Threads; ++i)
            {
                m_stacks.push_back(stack_factory.build());
            }
        }

        void set_symbols(const sak::mutable_storage& symbol_storage)
        {
            SuperCoder::set_symbols(symbol_storage);

            uint32_t thread_symbol_size = SuperCoder::symbol_size() / Threads;
            assert(thread_symbol_size * Threads == SuperCoder::symbol_size());

            for (uint32_t i = 0; i < Threads; ++i)
            {
                assert(m_stacks[i]->symbols() == SuperCoder::symbols());

                for (uint32_t j = 0; j < SuperCoder::symbols(); ++j)
                {
                    const uint8_t* symbol_j = SuperCoder::symbol(j);
                    auto s = sak::storage(symbol_j + (i * thread_symbol_size),
                                          thread_symbol_size);

                    m_stacks[i]->set_symbol(j, s);
                }
            }
        }

        uint32_t write_symbol(uint8_t* symbol_data, uint8_t* coefficients)
        {
            for(auto& c: m_coefficients)
            {
                std::copy_n(coefficients, SuperCoder::coefficient_vector_size(),
                            c.data());
            }

            std::thread t[Threads];

            for (uint32_t i = 0; i < Threads; ++i)
            {
                uint8_t* thread_symbol =
                    symbol_data + (i * m_stacks[i]->symbol_size());

                uint8_t* thread_coefficients = m_coefficients[i].data();

                auto& d = m_stacks[i];

                t[i] = std::thread([d, thread_symbol, thread_coefficients]()
                {
                    assert(d);
                    d->write_symbol(thread_symbol, thread_coefficients);
                });
            }

            for (uint32_t i = 0; i < Threads; ++i)
            {
                t[i].join();
            }

            return SuperCoder::symbol_size();
        }

        uint32_t write_uncoded_symbol(uint8_t* symbol_data, uint32_t index)
        {
            std::thread t[Threads];

            for (uint32_t i = 0; i < Threads; ++i)
            {
                uint8_t* thread_symbol =
                    symbol_data + (i * m_stacks[i]->symbol_size());

                auto& d = m_stacks[i];

                t[i] = std::thread([d, thread_symbol, index]()
                {
                    assert(d);
                    d->write_uncoded_symbol(thread_symbol, index);
                });
            }

            for (uint32_t i = 0; i < Threads; ++i)
            {
                t[i].join();
            }

            return SuperCoder::symbol_size();
        }

        uint32_t rank()
        {
            return m_stacks[0]->rank();
        }

        bool is_complete() const
        {
            return m_stacks[0]->is_complete();
        }

        std::vector<typename Stack::factory::pointer> m_stacks;
        std::vector<std::vector<uint8_t> > m_coefficients;
    };


    template<class Field, class TraceTag = kodo::disable_trace>
    class thread_encoder : public
        // Payload Codec API
        payload_encoder<
        // Codec Header API
        default_on_systematic_encoder<
        symbol_id_encoder<
        // Symbol ID API
        plain_symbol_id_writer<
        // Coefficient Generator API
        uniform_generator<
        // Encoder API
        thread_encoder_impl<8, shallow_full_vector_encoder<Field, TraceTag>,
        storage_aware_encoder<
        // Coefficient Storage API
        coefficient_value_access<
        coefficient_info<
        // Symbol Storage API
        partial_const_shallow_storage_layers<TraceTag,
        // Finite Field API
        finite_field_layers<Field,
        // Trace Layer
        trace_layer<TraceTag,
        // Final Layer
        final_layer
        > > > > > > > > > > > >
    {
    public:
        using factory = pool_factory<thread_encoder>;
    };

    template<class Field, class TraceTag = kodo::disable_trace>
    class sparse_thread_encoder : public
        // Payload Codec API
        payload_encoder<
        // Codec Header API
        default_on_systematic_encoder<
        symbol_id_encoder<
        // Symbol ID API
        plain_symbol_id_writer<
        // Coefficient Generator API
        sparse_uniform_generator<
        // Encoder API
        thread_encoder_impl<8, shallow_sparse_full_vector_encoder<Field, TraceTag>,
        storage_aware_encoder<
        // Coefficient Storage API
        coefficient_value_access<
        coefficient_info<
        // Symbol Storage API
        partial_const_shallow_storage_layers<TraceTag,
        // Finite Field API
        finite_field_layers<Field,
        // Trace Layer
        trace_layer<TraceTag,
        // Final Layer
        final_layer
        > > > > > > > > > > > >
    {
    public:
        using factory = pool_factory<sparse_thread_encoder>;
    };
}
}
