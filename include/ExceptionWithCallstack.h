#pragma once

namespace e186
{

	class ExceptionWithCallstack : public std::exception
	{
		std::string m_callstack;

		void SaveCallstack();

	public:
		explicit ExceptionWithCallstack(const std::string& what_arg);
		explicit ExceptionWithCallstack(const char* what_arg);

		const std::string& callstack() const;
	};

}
