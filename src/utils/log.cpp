#include "utils/log.hpp"
#include "geotiff_types/geo_exception.hpp"

#include <iostream>
#include <ostream>
#include <string>
#include <vector>
#include <iomanip>
#include <ctime>

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

bool Utils::OutputMessagesMgr::is_stream_valid()
{
    if (DISABLED == m_state)
    {
        return false;
    }
    if (nullptr == m_out)
    {
        throw GeoException("Output stream is null", 15);
    }
    return true;
}

void Utils::OutputMessagesMgr::change_color(const std::string& color)
{
    if (&std::cout == m_out)
    {
        *m_out << color;
    }
}

void Utils::OutputMessagesMgr::reset_color()
{
    if (&std::cout == m_out)
    {
        *m_out << RESET_CONSOLE;
    }
}

void Utils::OutputMessagesMgr::print_date()
{
    if (&std::cout == m_out)
    {
        return;
    }
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    *m_out << "[" << std::put_time(&tm, "%d-%m-%Y %H:%M:%S") << "]";
}

void Utils::OutputMessagesMgr::print_message(const std::string& prefix,
        const std::vector<std::string>& data)
{
    *m_out << prefix;
    for (const auto& value : data)
    {
        *m_out << value;
    }
}

void Utils::OutputMessagesMgr::print(const std::string& color,
        const std::string& prefix, const std::vector<std::string>& data)
{
    if (!is_stream_valid())
    {
        return;
    }
    change_color(color);
    print_date();
    print_message(prefix, data);
    reset_color();
    *m_out << std::endl;
}

void Utils::OutputMessagesMgr::info(const std::vector<std::string>& data)
{
    print(INFO_CONSOLE, " [@I] ", data);
}

void Utils::OutputMessagesMgr::warn(const std::vector<std::string>& data)
{
    print(WARN_CONSOLE, " [@W] ", data);
}

void Utils::OutputMessagesMgr::error(const std::vector<std::string>& data)
{
    print(ERROR_CONSOLE, " [@E] ", data);
}

Utils::OutputMessagesMgr::~OutputMessagesMgr()
{
}
