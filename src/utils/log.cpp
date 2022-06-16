#include "utils/log.hpp"
#include "geotiff_types/geo_exception.hpp"

#include <iostream>
#include <ostream>
#include <string>
#include <vector>

#define RESET_CONSOLE   "\033[0m"
#define INFO_CONSOLE    "\033[32m"
#define WARN_CONSOLE    "\033[33m"
#define ERROR_CONSOLE   "\033[31m"

Utils::Logs* Utils::Logger()
{
    return &Utils::Logs::instance();
}

Utils::Logs& Utils::Logs::instance()
{
    static Utils::Logs INSTANCE;
    return INSTANCE;
}

Utils::Logs::Logs()
    : m_state(ENABLED)
    , m_out(nullptr)
{
}

void Utils::Logs::enable()
{
    m_state = ENABLED;
}

void Utils::Logs::disable()
{
    m_state = DISABLED;
}

Utils::Logs::LoggerState Utils::Logs::get_state() const
{
    return m_state;
}

void Utils::Logs::set_stream(std::ostream* os)
{
    m_out = os;
}

void Utils::Logs::print(const std::string& color,
        const std::vector<std::string>& data)
{
    if (DISABLED == m_state)
    {
        return;
    }
    if (nullptr == m_out)
    {
        throw GeoException("output stream is null", 15);
    }
    if (&std::cout == m_out)
    {
        *m_out << color;
    }
    for (const auto& value : data)
    {
        *m_out << value;
    }
    if (&std::cout == m_out)
    {
        *m_out << RESET_CONSOLE;
    }
    *m_out << std::endl;
}

void Utils::Logs::info(const std::vector<std::string>& data)
{
    print(INFO_CONSOLE, data);
}

void Utils::Logs::warn(const std::vector<std::string>& data)
{
    print(WARN_CONSOLE, data);
}

void Utils::Logs::error(const std::vector<std::string>& data)
{
    print(ERROR_CONSOLE, data);
}

Utils::Logs::~Logs()
{
}
