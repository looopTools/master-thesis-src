#include "../result.hpp"
#include "../config.hpp"

#include <vector>
namespace master_thesis
{
namespace benchmark
{
class benchmark
{

    virtual std::vector<result> run() = 0;
    virtual result experiment() = 0;

protected:
    std::vector<result> m_results;

};
}
}
