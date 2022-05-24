#ifndef __BASE_SYSTEM_HPP__
#define __BASE_SYSTEM_HPP__

#include <string>

#include <boost/filesystem.hpp>

typedef   std::vector<std::string> Strings;
namespace fs = boost::filesystem;

namespace SysUtil
{

// execute cmd commands
int  cmd_exec(const std::string& cmd);

// filesystem commands
bool is_exists(const fs::path& path);
bool mkdir(const std::string& path);
bool remove(const fs::path& path);

} // system

#endif // __BASE_SYSTEM_HPP__
