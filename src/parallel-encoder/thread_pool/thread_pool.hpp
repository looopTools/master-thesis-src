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
#pragma once

#include <thread>
#include <cstdint>
#include <vector>
#include <functional>
// https://github.com/progschj/ThreadPool/blob/master/ThreadPool.h
namespace master_thesis
{
namespace thread_pool
{
class thread_pool
{
public:


    virtual std::thread get_thread() = 0;

    uint32_t available_threads()
    {
        return m_available_threads
    }

    uint32_t hadware_currency()
    {
        return static_cast<uint32_t>(std::thread:hardware_concurrency);
    }

private:
    std::vector<std::thread> workers
    uint32_t m_available_threads;

};
}
}
