#pragma once

#include <storage/storage.hpp>
#include <kodo_rlnc/full_vector_codes.hpp>


#include <cstdint>
#include <vector>
#include <algorithm>

#include <thread>
#include <mutex>
#include <atomic>

#include <chrono>

namespace master_thesis
{
namespace encoder
{
class threaded_encoder
{
    using rlnc_encoder = kodo_rlnc::shallow_full_vector_encoder<fifi::binary8>;
public:
    threaded_encoder(uint32_t symbols, uint32_t symbol_size,
                     const storage::const_storage& symbol_storage) :
        m_completed(false), m_symbols(symbols), m_symbol_size(symbol_size)
    {

        m_threads = std::vector<std::thread>(std::thread::hardware_concurrency());
        m_remainder = symbols % m_threads.size();
        m_coefficients = symbols / m_threads.size();

        rlnc_encoder::factory encoder_factory(symbols, symbol_size);

        m_encoders = std::vector<std::shared_ptr<rlnc_encoder>>(m_threads.size());
        for (uint32_t i = 0; i < m_encoders.size(); ++i)
        {
            auto encoder = encoder_factory.build();
            encoder->set_const_symbols(symbol_storage);
            encoder->set_systematic_off();
            m_encoders.at(i) = encoder;
        }

        m_result = std::vector<std::vector<uint8_t>>(symbols,
                                                     std::vector<uint8_t>((m_encoders.at(0))->payload_size()));

    }

    void encode()
    {

        for (uint32_t i = 0; i < m_threads.size(); ++i)
        {

            auto encoder = m_encoders.at(i);


            if (i == m_threads.size() - 1 && m_remainder != 0)
            {
                auto coefficients = m_coefficients + m_remainder;
                m_threads.at(i) = std::thread([this, encoder, coefficients]()
                                           {
                        std::vector<std::vector<uint8_t>> payloads(coefficients, std::vector<uint8_t>(encoder->payload_size()));
                        for (uint32_t j = 0; j < coefficients; ++j)
                        {
                            encoder->write_payload(payloads[j].data());
                        }

                        std::lock_guard<std::mutex> lock(this->m_mutex);
                        for (auto element : payloads) {
                            std::swap(v.back)
                        }
                        this->m_result.insert(std::end(this->m_result),
                                              std::begin(payloads),
                                              std::end(payloads));

                                           });
            }
            else
            {
                m_threads.at(i) = std::thread([this, encoder, i]()
                                           {
                                               auto start = std::chrono::high_resolution_clock::now();
                                               std::vector<std::vector<uint8_t>> payloads(this->m_coefficients, std::vector<uint8_t>(encoder->payload_size()));
                                               for (uint32_t j = 0; j < this->m_coefficients; ++j)
                                               {
                                                   encoder->write_payload(payloads[j].data());
                                               }

                                               std::lock_guard<std::mutex> lock(this->m_mutex);
                                               this->m_result.insert(std::end(this->m_result),
                                                                     std::begin(payloads),
                                                                     std::end(payloads));
                                               auto end = std::chrono::high_resolution_clock::now();
                                               auto c_start = std::chrono::duration_cast<std::chrono::microseconds>(start.time_since_epoch());
                                               auto c_end = std::chrono::duration_cast<std::chrono::microseconds>(end.time_since_epoch());

                                               std::cout << i << " " << ((c_end.count() - c_start.count()) / 1000.0) << std::endl;
                            });
            }

        }


        for (uint32_t i = 0; i < m_threads.size(); ++i)
        {
            m_threads[i].join();
        }
        m_completed = true;;

    }

    std::vector<std::vector<uint8_t>> result()
    {
        return m_result;
    }

    bool completed()
    {
        return m_completed;
    }

private:
    std::vector<std::shared_ptr<rlnc_encoder>> m_encoders;
    std::vector<std::vector<uint8_t>> m_result;
    std::vector<std::thread> m_threads;
    uint32_t remainder;
    uint32_t m_coefficients;
    std::mutex m_mutex;
    bool m_completed;
    uint32_t m_symbols;
    uint32_t m_symbol_size;
    uint32_t m_remainder;
};
}
}
