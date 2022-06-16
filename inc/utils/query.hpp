#ifndef __UTILS_QUERY_HPP__
#define __UTILS_QUERY_HPP__

#include <string>

namespace Utils {

class QueryMessagesMgr
{
public:
    typedef enum { NONE, YES, NO } QueryValueYN;

public:
    static QueryMessagesMgr& instance();

    QueryMessagesMgr(const QueryMessagesMgr&) = delete;
    QueryMessagesMgr& operator=(const QueryMessagesMgr&) = delete;
    ~QueryMessagesMgr();

    bool ask_for_yn(const std::string& message,
                    const QueryValueYN defval = QueryValueYN::NONE);

private:
    QueryMessagesMgr();
    void create_msg(std::string& message, const QueryValueYN defval);
    bool process_msg(const std::string& message, const QueryValueYN defval);
    bool check_input(const std::string& input, const QueryValueYN defval,
            bool& ret);
};

QueryMessagesMgr* Querier();

} // Utils

#endif // __UTILS_QUERY_HPP__
