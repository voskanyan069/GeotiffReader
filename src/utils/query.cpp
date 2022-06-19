#include "utils/query.hpp"
#include "geotiff_types/geo_exception.hpp"

#include <iostream>
#include <string>
#include <algorithm>

Utils::QueryMessagesMgr* Utils::Querier()
{
    return &Utils::QueryMessagesMgr::instance();
}

Utils::QueryMessagesMgr& Utils::QueryMessagesMgr::instance()
{
    static Utils::QueryMessagesMgr INSTANCE;
    return INSTANCE;
}

Utils::QueryMessagesMgr::QueryMessagesMgr()
{
}

void Utils::QueryMessagesMgr::create_msg(std::string& message,
        const QueryValueYN defval)
{
    switch (defval)
    {
        case Utils::QueryMessagesMgr::QueryValueYN::YES:
            message += " [Y/n]: ";
            break;
        case Utils::QueryMessagesMgr::QueryValueYN::NO:
            message += " [y/N]: ";
            break;
        case Utils::QueryMessagesMgr::QueryValueYN::NONE:
            message += " [y/n]: ";
            break;
    }
}

bool Utils::QueryMessagesMgr::check_input(const std::string& input,
        const Utils::QueryMessagesMgr::QueryValueYN defval,
        bool& ret)
{
    std::string msg = input;
    if (msg.empty() && Utils::QueryMessagesMgr::QueryValueYN::NONE != defval)
    {
        ret = Utils::QueryMessagesMgr::QueryValueYN::YES == defval;
        return true;
    }
    if (msg.empty())
    {
        return false;
    }
    std::transform(msg.begin(), msg.end(), msg.begin(), tolower);
    if ("y" == msg || "yes" == msg)
    {
        ret = true;
        return true;
    }
    else if ("n" == msg || "no" == msg)
    {
        ret = false;
        return true;
    }
    return false;
}

bool Utils::QueryMessagesMgr::process_msg(const std::string& message,
        const Utils::QueryMessagesMgr::QueryValueYN defval)
{
    bool ret = false;
    bool res = false;
    std::string query_msg = message;
    create_msg(query_msg, defval);
    do
    {
        std::string input;
        std::cout << query_msg;
        std::getline(std::cin, input);
        res = check_input(input, defval, ret);
    } while (!res);
    return ret;
}

bool Utils::QueryMessagesMgr::ask_for_yn(const std::string& message,
        const Utils::QueryMessagesMgr::QueryValueYN defval)
{
    bool res = process_msg(message, defval);
    return res;
}

Utils::QueryMessagesMgr::~QueryMessagesMgr()
{
}
