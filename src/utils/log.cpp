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

Utils::OutputMessagesMgr* Utils::Logger()
{
    return &Utils::OutputMessagesMgr::instance();
}

Utils::OutputMessagesMgr& Utils::OutputMessagesMgr::instance()
{
    static Utils::OutputMessagesMgr INSTANCE;
    return INSTANCE;
}

Utils::OutputMessagesMgr::OutputMessagesMgr()
    : m_state(ENABLED)
    , m_out(nullptr)
{
}

void Utils::OutputMessagesMgr::enable()
{
    m_state = ENABLED;
}

void Utils::OutputMessagesMgr::disable()
{
    m_state = DISABLED;
}

Utils::OutputMessagesMgr::LoggerState Utils::OutputMessagesMgr::get_state() const
{
    return m_state;
}

void Utils::OutputMessagesMgr::set_stream(std::ostream* os)
{
    m_out = os;
}

void Utils::OutputMessagesMgr::print(const std::string& color,
        const std::vector<std::string>& data)
{
    if (DISABLED == m_state)
    {
        return;
    }
    if (nullptr == m_out)
    {
        throw GeoException("Output stream is null", 15);
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

void Utils::OutputMessagesMgr::info(const std::vector<std::string>& data)
{
    print(INFO_CONSOLE, data);
}

void Utils::OutputMessagesMgr::warn(const std::vector<std::string>& data)
{
    print(WARN_CONSOLE, data);
}

void Utils::OutputMessagesMgr::error(const std::vector<std::string>& data)
{
    print(ERROR_CONSOLE, data);
}

Utils::OutputMessagesMgr::~OutputMessagesMgr()
{
}
