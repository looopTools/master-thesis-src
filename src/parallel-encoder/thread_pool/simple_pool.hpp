#pragma once

#include <vector>
#include <memory>

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
                            if (this->m_stop && this->m_task.empty())
                            {
                                return;
                            }

                            // Move first enqueued task to the task var
                            task = std::move(this->m_task.front());
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
        using return_type = typename std::result_of<Function(args...)>::type;

        auto task = std::make_shared<>
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
    std::queue<std::function<void()>> m_task;

    std::mutex task_mutex;


    uint32_t m_coefficients;
    uint64_t m_available_threads;
    bool m_stop;

}
}
}
