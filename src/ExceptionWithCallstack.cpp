#include "ExceptionWithCallstack.h"

namespace e186
{
	ExceptionWithCallstack::ExceptionWithCallstack(const std::string& what_arg)
		: std::runtime_error(what_arg.c_str())
	{
		SaveCallstack();
	}

	ExceptionWithCallstack::ExceptionWithCallstack(const char* what_arg)
		: std::runtime_error(what_arg)
	{	
		SaveCallstack();
	}

	void ExceptionWithCallstack::SaveCallstack()
	{
		m_callstack = GetCurrentCallstack();
	}
	
	const std::string& ExceptionWithCallstack::callstack() const
	{
		return m_callstack;
	}
}
