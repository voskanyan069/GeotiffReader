#ifndef __BASE_CMD_ARGUMENT_HPP__
#define __BASE_CMD_ARGUMENT_HPP__

#include <map>
#include <vector>
#include <string>
#include <memory>

#include "base/argument_base.hpp"

template <typename T>
class CMDArgument : ArgumentBase
{
public:
	CMDArgument(const T& rhs) : m_value(rhs) {}
	const T& get() const { return m_value; }

private:
	const T m_value;
};

class CMDArguments
{
public:
	static CMDArguments& instance();

	CMDArguments(const CMDArguments&) = delete;
	CMDArguments& operator=(const CMDArguments&) = delete;

	const ArgumentBase* find(const std::string& name);
	void set_argument(const std::string& name, const bool value);
	void set_argument(const std::string& name, ArgumentBase* arg);

private:
	CMDArguments();
	~CMDArguments();

private:
	std::map<std::string, ArgumentBase*> arguments;
};

typedef CMDArgument<int>         CMDIntArgument;
typedef CMDArgument<std::string> CMDStrArgument;

#endif // __BASE_CMD_ARGUMENT_HPP__
