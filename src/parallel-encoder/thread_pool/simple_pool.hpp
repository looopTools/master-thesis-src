#pragma once

#include <vector>
#include <memory>

// https://github.com/progschj/ThreadPool/blob/master/ThreadPool.h

namespace master_thesis
{
namespace thread_pool
{
class simple_pool
{

public:
    // coefficients: Amount of packets = g + r
    simple_pool(uint32_t coefficients) : m_coefficients(coefficients),
                                         m_available_threads(coefficients),
                                         m_stop(false)
    {
        for (uint32_t = 0; i < coefficients; ++i)
        {
            // Intialise an object in the end of the queue
            m_threads.emplace_back(
                [this]
                {
                    for(;;)
                    {
                        std::function<void()> task;

                        {
                            std::unique_lock<std::mutex> lock(this->queue_mutex);


                            this->condition.wait(lock, [this]
                                                 {
                                                     return this->m_stop
                                                         || this.m_tasks.empty();
                                                 });

                            // If the queue is empty we have nothing to do
                            if (this->m_stop && this->m_tasks.empty())
                            {
                                return;
                            }

                            // Move first enqueued task to the task var
                            task = std::move(this->m_tasks.front());
                            this->tasks.pop()
                        }

                        // Call the function currently being hold in the task variable
                        task();

                    }
                }
                );
        }
    }

    template<class Function, class... Args>
    auto enqueue(Function&& f, Args&&... args)
    {
        using return_type = typename std::result_of<function(args...)>::type

        using return_type = typename std::result_of<Function(args...)>::type;

        auto task = std::make_shared<std::package_task<return_type>>(
            std::bind(std::forward<Function>(f), std::<Args>(args)...));

        std::future<return_type> res = m_task->get_future();
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            if (stop)
            {
                throw std::runtime_error("enqueue on stopped Thread pool");
            }

            m_tasks.emplace([task](){(*task)();})

            // Notify one waiting thread that something has been enqueued
            m_condition.notify_one()
        }

    }

    uint32_t available_threads()
    {
        return m_available_threads;
    }

    void stop()
    {
        for (auto thread : threads)
        {
            thread.join()
        }
    }

private:
    std::vector<std::unique_ptr<std::thread>> m_threads;
    // Task which are enqued
    std::queue<std::function<void()>> m_tasks;

    std::mutex m_task_mutex;

    std::condition_variable m_condition;

    uint32_t m_coefficients;
    uint64_t m_available_threads;
    bool m_stop;

}
}
}
