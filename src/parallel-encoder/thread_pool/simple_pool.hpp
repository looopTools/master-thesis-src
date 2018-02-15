#include "thread_pool.hpp"
namespace master_thesis
{
namespace thread_pool
{
    class simple_pool : public thread_pool
    {
    public:
        simple_pool(uint32_t threads);

    };
}
}
