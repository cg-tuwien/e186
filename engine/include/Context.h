#pragma once

namespace e186
{

	class Context
	{
		bool m_good;

		static void glfw_error_callback(int error, const char* description);

	public:
		Context(int glMajorVersion = 4, int glMinorVersion = 5);
		~Context();
		
		static GLFWwindow* create_window(int width, int height, GLFWmonitor* monitor = nullptr, const char* name = "E186 Main Window");
		static void destroy_window(GLFWwindow* wnd);

		bool good() const {
			return m_good;
		}
	};

}
