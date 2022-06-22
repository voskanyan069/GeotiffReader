#ifndef __UTILS_LOG_HPP__
#define __UTILS_LOG_HPP__

#include <string>
#include <vector>
#include <ostream>

namespace Utils {

class OutputMessagesMgr
{
    typedef enum { ENABLED, DISABLED } LoggerState;

public:
    static OutputMessagesMgr& instance();

    OutputMessagesMgr(const OutputMessagesMgr&) = delete;
    OutputMessagesMgr& operator=(const OutputMessagesMgr&) = delete;
    ~OutputMessagesMgr();

    void enable();
    void disable();
    LoggerState get_state() const;
    void set_stream(std::ostream* os);
    void info(const std::vector<std::string>& data);
    void warn(const std::vector<std::string>& data);
    void error(const std::vector<std::string>& data);

private:
    OutputMessagesMgr();
    bool is_stream_valid();
    void change_color(const std::string& color);
    void reset_color();
    void print_date();
    void print_message(const std::string& prefix,
            const std::vector<std::string>& data);
    void print(const std::string& color, const std::string& prefix,
            const std::vector<std::string>& data);

private:
    LoggerState m_state;
    std::ostream* m_out;
};

OutputMessagesMgr* Logger();

} // Utils

#endif // __UTILS_LOG_HPP__
