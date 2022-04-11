#ifndef __BASE_ARGUMENT_BASE_HPP__
#define __BASE_ARGUMENT_BASE_HPP__

template <typename T>
class CMDArgument;

class ArgumentBase
{
public:
    virtual ~ArgumentBase()
	{
	}

	template <typename T>
    const T& get() const
	{
		return ((const CMDArgument<T>*)(this))->get();
	}
};

#endif // __BASE_ARGUMENT_BASE_HPP__
