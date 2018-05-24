#include <cstdint>

namespace master_thesis
{
class encoder
{
public:
    virtual void setup() = 0;
    virtual void start() = 0;
    virtual bool completed() = 0;

protected:

    uint32_t m_symbols;
    uint32_t m_symbol_size;
    uint32_t m_completed;


};
}
