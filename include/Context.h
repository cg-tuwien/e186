#pragma once

namespace e186
{

	class Context
	{
		static void glfw_error_callback(int error, const char* description);

	public:
		Context(int glMajorVersion = 4, int glMinorVersion = 6);
		~Context();
		
		static Context* current() { return Context::g_current; }

		static GLFWwindow* create_window(int width, int height, GLFWmonitor* monitor = nullptr, const char* name = "E186 Main Window");
		static void destroy_window(GLFWwindow* wnd);

		bool good() const { return m_good; }
		int gl_major_version() const { return m_gl_major_version; }
		int gl_minor_version() const { return m_gl_minor_version; }
		int is_core_profile() const { return m_is_core_profile; }

	private:
		static Context* g_current;
		bool m_good;
		int m_gl_major_version;
		int m_gl_minor_version;
		bool m_is_core_profile;
	};

}
