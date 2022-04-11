#include <iostream>

#include "base/system.hpp"

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

void SysUtil::info(const std::string& message)
{
	std::cout << " [@I] " << message << std::endl;
}

void SysUtil::warn(const std::string& message)
{
	std::cout << " [@W] " << message << std::endl;
}

void SysUtil::error(const std::string& message)
{
	std::cout << " [@E] " << message << std::endl;
	std::exit(-1);
}

void SysUtil::info(const Strings& messages)
{
	std::cout << " [@I] ";
	for (const auto msg : messages)
	{
		std::cout << msg;
	}
	std::cout << std::endl;
}

void SysUtil::warn(const Strings& messages)
{
	std::cout << " [@W] ";
	for (const auto msg : messages)
	{
		std::cout << msg;
	}
	std::cout << std::endl;
}

void SysUtil::error(const Strings& messages)
{
	std::cout << " [@E] ";
	for (const auto msg : messages)
	{
		std::cout << msg;
	}
	std::cout << std::endl;
	std::exit(-1);
}
