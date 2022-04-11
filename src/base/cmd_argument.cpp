#include "base/cmd_argument.hpp"
#include "base/argument_base.hpp"

CMDArguments::CMDArguments()
{
}

CMDArguments& CMDArguments::instance()
{
	static CMDArguments INSTANCE;
	return INSTANCE;
}

const ArgumentBase* CMDArguments::find(const std::string& name)
{
    const auto& it = arguments.find(name);
    if (it == arguments.end())
    {
        throw std::runtime_error(name + " not found");
    }
    return it->second;
}

CMDArguments::~CMDArguments()
{
}
