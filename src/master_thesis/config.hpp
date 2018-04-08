#include <cstdint>
#include <string>

class config
{
public:
    config(){}

    void set_parameters(uint32_t threads, uint32_t symbols, uint32_t symbol_size)
    {
        m_threads = threads;
        m_symbols = symbols;
        m_symbol_size = symbol_size;
    }
    uint32_t symbols()
    {
        return m_symbols;
    }

    uint32_t symbol_size()
    {
        return m_symbol_size;
    }

    uint32_t threads()
    {
        return m_threads;
    }

    std::string to_string()
    {

        std::string configuration = std::to_string(m_threads) + "," + std::to_string(m_symbols) + "," + std::to_string(m_symbol_size);
        return configuration;
    }
private:
    uint32_t m_symbols;
    uint32_t m_symbol_size;
    uint32_t m_threads;
};
