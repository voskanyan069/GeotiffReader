#include <iostream>

#include "base/system.hpp"

#define RESET_CONSOLE	"\033[0m"
#define INFO_CONSOLE	"\033[32m"
#define WARN_CONSOLE	"\033[33m"
#define ERROR_CONSOLE	"\033[31m"

int SysUtil::cmd_exec(const std::string& cmd)
{
	int rc = system(cmd.c_str());
	return rc;
}

bool SysUtil::is_exists(const fs::path& path)
{
	bool exists = fs::exists(path);
	return exists;
}

bool SysUtil::mkdir(const std::string& path)
{
	boost::system::error_code ec;
	fs::create_directory(path, ec);
	if (ec)
	{
		warn(ec.message());
		return false;
	}
	return true;
}

bool SysUtil::remove(const fs::path& path)
{
	boost::system::error_code ec;
	fs::remove(path, ec);
	if (ec)
	{
		warn(ec.message());
		return false;
	}
	return true;
}

void SysUtil::info(const std::string& message)
{
	std::cout << INFO_CONSOLE << " [@I] " << message
		<< RESET_CONSOLE << std::endl;
}

void SysUtil::warn(const std::string& message)
{
	std::cout << WARN_CONSOLE << " [@W] " << message
		<< RESET_CONSOLE << std::endl;
}

void SysUtil::error(const std::string& message)
{
	std::cout << ERROR_CONSOLE << " [@E] " << message
		<< RESET_CONSOLE << std::endl;
	std::exit(-1);
}

void SysUtil::info(const Strings& messages)
{
	std::cout << INFO_CONSOLE << " [@I] ";
	for (const auto msg : messages)
	{
		std::cout << msg;
	}
	std::cout << RESET_CONSOLE << std::endl;
}

void SysUtil::warn(const Strings& messages)
{
	std::cout << WARN_CONSOLE << " [@W] ";
	for (const auto msg : messages)
	{
		std::cout << msg;
	}
	std::cout << RESET_CONSOLE << std::endl;
}

void SysUtil::error(const Strings& messages)
{
	std::cout << ERROR_CONSOLE << " [@E] ";
	for (const auto msg : messages)
	{
		std::cout << msg;
	}
	std::cout << RESET_CONSOLE << std::endl;
	std::exit(-1);
}
