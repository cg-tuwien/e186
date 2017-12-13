#include "ExceptionWithCallstack.h"
#include <Windows.h>
#include <DbgHelp.h>
#include <sstream>

namespace e186
{
	ExceptionWithCallstack::ExceptionWithCallstack(const std::string& what_arg)
		: std::exception(what_arg.c_str())
	{
		SaveCallstack();
	}

	ExceptionWithCallstack::ExceptionWithCallstack(const char* what_arg)
		: std::exception(what_arg)
	{	
		SaveCallstack();
	}

	void ExceptionWithCallstack::SaveCallstack()
	{
		void         * stack[100];

		HANDLE process = GetCurrentProcess();

		SymInitialize(process, nullptr, TRUE);

		unsigned short frames = CaptureStackBackTrace(0, 100, stack, nullptr);
		SYMBOL_INFO * symbol = static_cast<SYMBOL_INFO *>(calloc(sizeof(SYMBOL_INFO) + 256 * sizeof(char), 1));
		symbol->MaxNameLen = 255;
		symbol->SizeOfStruct = sizeof(SYMBOL_INFO);

		m_callstack = std::string();
		for (unsigned int i = 0; i < frames; i++)
		{
			SymFromAddr(process, reinterpret_cast<DWORD64>(stack[i]), nullptr, symbol);
			
			std::stringstream ss;
			ss << frames - i - 1 << ": " << symbol->Name << " (0x" << std::hex << symbol->Address << ")" << std::endl;
			m_callstack += ss.str();

			if (strcmp(symbol->Name, "main") == 0) 
			{
				break;
			}
		}

		free(symbol);
	}
	
	const std::string& ExceptionWithCallstack::callstack() const
	{
		return m_callstack;
	}
}
