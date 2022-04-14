#ifndef __BASE_SYSTEM_HPP__
#define __BASE_SYSTEM_HPP__

#include <string>
#include <vector>

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

// logging messages
void info(const std::string& messages);
void warn(const std::string& messages);
void error(const std::string& messages);
void info(const Strings& messages);
void warn(const Strings& messages);
void error(const Strings& messages);

} // system

#endif // __BASE_SYSTEM_HPP__
