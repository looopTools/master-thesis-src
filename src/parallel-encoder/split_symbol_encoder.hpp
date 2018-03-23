#pragma once

#include <cstdint>

#include <kodo_rlnc/full_vector_codes.hpp>

namespace parallel_encoder
{
template<uint32_t Threads, class Stack, class SuperCoder>
class thread_encoder_impl : public SuperCoder
{

};
}
