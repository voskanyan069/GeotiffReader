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
		return false;
	}
	return true;
}
