#include "Context.h"

namespace e186
{
	void Context::glfw_error_callback(int error, const char* description)
	{
		std::cout << "glfw_error_callback #" << error << ": " << description << std::endl;
	}
	
	Context::Context(int glMajorVersion, int glMinorVersion)
		: m_good(false)
	{
		log_verbose("Creating context...");

		// Setting an error callback
		glfwSetErrorCallback(glfw_error_callback);

		// Initializing GLFW
		if (!glfwInit())
		{
			log_error("glfwInit failed");
		}
		else
		{	
			// ======= GLFW: Creating a window and context
			glfwWindowHint(GLFW_SRGB_CAPABLE, GLFW_TRUE);
			glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, glMajorVersion);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, glMinorVersion);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			glfwWindowHint(GLFW_SAMPLES, 4); // antialiasing (MSAA)
#ifdef _DEBUG
			glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif
		
			m_good = true;
		}
	}

	GLFWwindow* Context::create_window(int width, int height, GLFWmonitor* monitor, const char* name)
	{
		auto* window = glfwCreateWindow(width, height, name, monitor, nullptr);
		if (!window)
		{
			log_error("Window or OpenGL context creation failed");
		}
		return window;
	}

	void Context::destroy_window(GLFWwindow* wnd)
	{
		if (wnd)
		{
			glfwDestroyWindow(wnd);
		}
	}

	Context::~Context()
	{
		glfwTerminate();
	}

}
