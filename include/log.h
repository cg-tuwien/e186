#pragma once

namespace e186
{
	// LOG-LEVELS:
	// 0 ... nothing (except debug messages in DEBUG-mode)
	// 1 ... error messages only
	// 2 ... errors and warnings
	// 3 ... errors, warnings and infos
	// 4 ... everything

	#if !defined(LOG_LEVEL)
	#define LOG_LEVEL 3
	#endif

	#if LOG_LEVEL > 0
	#define log_error(...)		do { printf("ERROR:   "); printf(__VA_ARGS__); printf("\n"); } while(0)
	#else
	#define log_error(...)
	#endif

	#if LOG_LEVEL > 1
	#define log_warning(...)	do { printf("WARNING: "); printf(__VA_ARGS__); printf("\n"); } while(0)
	#else 
	#define log_warning(...)
	#endif

	#if LOG_LEVEL > 2
	#define log_info(...)		do { printf("INFO:    "); printf(__VA_ARGS__); printf("\n"); } while(0)
	#else
	#define log_info(...)
	#endif

	#if LOG_LEVEL > 3
	#define log_verbose(...)	do { printf("VERBOSE: "); printf(__VA_ARGS__); printf("\n"); } while(0)
	#else 
	#define log_verbose(...)
	#endif

	#ifdef _DEBUG
	#define log_debug(...)		do { printf("DEBUG:   "); printf(__VA_ARGS__); printf("\n"); } while(0)
	#else
	#define log_debug(...)      	
	#endif

	#if defined(_DEBUG) && LOG_LEVEL > 3
	#define log_debug_verbose(...)		do { printf("DBGVRBSE:"); printf(__VA_ARGS__); printf("\n"); } while(0)
	#else
	#define log_debug_verbose(...)      	
	#endif

	bool check_gl_error(const char* location);

	std::string matrix_to_string(const glm::mat4& m);
	std::string matrix_to_string(const glm::mat3& m);
	std::string matrix_to_string_compact(const glm::mat4& m);
	std::string matrix_to_string_compact(const glm::mat3& m);

	std::string vector_to_string(const glm::vec2& v);
	std::string vector_to_string(const glm::vec3& v);
	std::string vector_to_string(const glm::vec4& v);

	const char* fourccToString(unsigned int fourcc);

	std::string GetCurrentCallstack();
}
