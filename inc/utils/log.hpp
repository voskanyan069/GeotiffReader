#ifndef __UTILS_LOG_HPP__
#define __UTILS_LOG_HPP__

#include <string>
#include <vector>
#include <ostream>

namespace Utils {

class Logs
{
private:
    typedef enum { ENABLED, DISABLED } LoggerState;

public:
    static Logs& instance();

    Logs(const Logs&) = delete;
    Logs& operator=(const Logs&) = delete;
    ~Logs();

    void enable();
    void disable();
    LoggerState get_state() const;
    void set_stream(std::ostream* os);
    void info(const std::vector<std::string>& data);
    void warn(const std::vector<std::string>& data);
    void error(const std::vector<std::string>& data);

private:
    Logs();
    void print(const std::string& color, const std::vector<std::string>& data);

private:
    LoggerState m_state;
    std::ostream* m_out;
};

Logs* Logger();

} // Utils

#endif // __UTILS_LOG_HPP__
