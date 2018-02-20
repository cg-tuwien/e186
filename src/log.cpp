#include "log.h"
#if defined(_WIN32) && !defined(DISABLE_PRINT_CALLSTACK)
#include <Windows.h>
#include <DbgHelp.h>
#include <sstream>
#endif

namespace e186
{
	bool check_gl_error(const char* location)
	{
		bool has_error = false;
		GLenum err;
		while ((err = glGetError()) != GL_NO_ERROR)
		{
			log_error("GL-Error in %s glError(0x%x)", location, err);
			has_error = true;
		}
		return has_error;
	}

	std::string matrix_to_string(const glm::mat4& m)
	{
		char buf[128];
		sprintf_s(buf, 128, 
			"\t%.3f\t%.3f\t%.3f\t%.3f\n\t%.3f\t%.3f\t%.3f\t%.3f\n\t%.3f\t%.3f\t%.3f\t%.3f\n\t%.3f\t%.3f\t%.3f\t%.3f\n",
			m[0][0], m[0][1], m[0][2], m[0][3],
			m[1][0], m[1][1], m[1][2], m[1][3],
			m[2][0], m[2][1], m[2][2], m[2][3],
			m[3][0], m[3][1], m[3][2], m[3][3]);
		return buf;
	}

	std::string matrix_to_string(const glm::mat3& m)
	{
		char buf[128];
		sprintf_s(buf, 128, 
			"\t%.3f\t%.3f\t%.3f\n\t%.3f\t%.3f\t%.3f\n\t%.3f\t%.3f\t%.3f\n",
			m[0][0], m[0][1], m[0][2],
			m[1][0], m[1][1], m[1][2],
			m[2][0], m[2][1], m[2][2]);
		return buf;
	}

	std::string matrix_to_string_compact(const glm::mat4& m)
	{
		char buf[128];
		sprintf_s(buf, 128, 
			"{{%.2f, %.2f, %.2f, %.2f}, {%.2f, %.2f, %.2f, %.2f}, {%.2f, %.2f, %.2f, %.2f}, {%.2f, %.2f, %.2f, %.2f}}\n",
			m[0][0], m[0][1], m[0][2], m[0][3],
			m[1][0], m[1][1], m[1][2], m[1][3],
			m[2][0], m[2][1], m[2][2], m[2][3],
			m[3][0], m[3][1], m[3][2], m[3][3]);
		return buf;
	}

	std::string matrix_to_string_compact(const glm::mat3& m)
	{
		char buf[128];
		sprintf_s(buf, 128, 
			"{{%.2f, %.2f, %.2f}, {%.2f, %.2f, %.2f}, {%.2f, %.2f, %.2f}}\n",
			m[0][0], m[0][1], m[0][2],
			m[1][0], m[1][1], m[1][2],
			m[2][0], m[2][1], m[2][2]);
		return buf;
	}


	std::string vector_to_string(const glm::vec2& v)
	{
		char buf[64];
		sprintf_s(buf, 64, "(%.2f, %.2f)", v.x, v.y);
		return buf;
	}

	std::string vector_to_string(const glm::vec3& v)
	{
		char buf[64];
		sprintf_s(buf, 64, "(%.2f, %.2f, %.2f)", v.x, v.y, v.z);
		return buf;
	}

	std::string vector_to_string(const glm::vec4& v)
	{
		char buf[64];
		sprintf_s(buf, 64, "(%.2f, %.2f, %.2f, %.2f)", v.x, v.y, v.z, v.w);
		return buf;
	}

	char fourccBuf[8];
	const char* fourccToString(unsigned int fourcc)
	{
		fourccBuf[3] = static_cast<char>(0x000000FF & fourcc);
		fourccBuf[2] = static_cast<char>(0x000000FF & (fourcc >> 8));
		fourccBuf[1] = static_cast<char>(0x000000FF & (fourcc >> 16));
		fourccBuf[0] = static_cast<char>(0x000000FF & (fourcc >> 24));

		// convert 000000000 to spaces
		for (int i = 0; i<4; i++)
		{
			if (0 == fourccBuf[i])
				fourccBuf[i] = ' ';
		}

		fourccBuf[4] = 0;
		return fourccBuf;
	}

	std::string GetCurrentCallstack()
	{
#if defined(_WIN32) && defined (_DBGHELP_)
		void         * stack[100];

		HANDLE process = GetCurrentProcess();

		SymInitialize(process, nullptr, TRUE);

		unsigned short frames = CaptureStackBackTrace(0, 100, stack, nullptr);
		SYMBOL_INFO * symbol = static_cast<SYMBOL_INFO *>(calloc(sizeof(SYMBOL_INFO) + 256 * sizeof(char), 1));
		symbol->MaxNameLen = 255;
		symbol->SizeOfStruct = sizeof(SYMBOL_INFO);

		auto callstack = std::string();
		for (unsigned int i = 0; i < frames; i++)
		{
			SymFromAddr(process, reinterpret_cast<DWORD64>(stack[i]), nullptr, symbol);

			std::stringstream ss;
			ss << frames - i - 1 << ": " << symbol->Name << " (0x" << std::hex << symbol->Address << ")" << std::endl;
			callstack += ss.str();

			if (strcmp(symbol->Name, "main") == 0)
			{
				break;
			}
		}

		free(symbol);
		return callstack;
#else
		return std::string();
#endif
	}
}