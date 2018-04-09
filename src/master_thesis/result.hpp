#include <cstdint>
#include <chrono>
#include <sstream>
#include <string>

namespace master_thesis
{
class result
{
public:
    result(std::chrono::microseconds start, std::chrono::microseconds end,
           uint32_t threads, uint32_t symbols, uint32_t symbol_size) :
        m_start(start), m_end(end), m_threads(threads), m_symbols(symbols),
        m_symbol_size(symbol_size){ }

    uint64_t latency()
    {
        return static_cast<uint64_t>(m_end.count() - m_start.count());
    }


    std::string to_string()
    {
        std::stringstream ss;
        ss << m.start.count() << "," << m.end.count() << "," << latency() << ","
           << m_threads << "," << m_symbols << "," << m_symbol_size;
        return ss.str();
    }
private:
    uint32_t m_threads;
    uint32_t m_symbols;
    uint32_t m_symbol_size;
    std::chrono::microseconds m_start;
    std::chrono::microseconds m_end;
};
}
