#include "Engine.h"
#include "SampleSceneTessIco.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace e186
{
	Engine* Engine::current(nullptr);

	Engine::Engine(GLFWwindow* mainWindow) :
		m_mainWindow(mainWindow),
		m_running(false),
		m_main_wnd_width(0),
		m_main_wnd_height(0),
		m_main_wnd_aspectRatio(0),
		m_root_scene_generator_func([]() { return nullptr; }),
		m_next_is_root(false),
		m_current_is_root(false)
	{
		glfwGetFramebufferSize(m_mainWindow, &m_main_wnd_width, &m_main_wnd_height);
		m_main_wnd_aspectRatio = static_cast<float>(m_main_wnd_width) / static_cast<float>(m_main_wnd_height);

		// Receiving input events
		glfwSetMouseButtonCallback(m_mainWindow, glfw_mouse_button_callback);
		glfwSetCursorPosCallback(m_mainWindow, glfw_cursor_pos_callback);
		glfwSetScrollCallback(m_mainWindow, glfw_scroll_callback);
		glfwSetKeyCallback(m_mainWindow, glfw_key_callback);
		glfwSetCharCallback(m_mainWindow, glfw_char_callback);

		// Window size callback
		glfwSetWindowSizeCallback(m_mainWindow, glfw_windowsize_callback);

		// Making the OpenGL context current
		glfwMakeContextCurrent(m_mainWindow);
		
		// If you are using an extension loader library to access modern OpenGL then this is when to initialize it, as the loader needs a current context to load from.
		gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

//#ifdef _DEBUG
		// Register GL debug callback if we are in DEBUG mode
		glDebugMessageCallback(Engine::opengl_debug_message_callback, nullptr);
		// Enable synchronous callback. This ensures that the callback function is called right after an error has occurred. 
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
//#endif

		// glfwSwapInterval(1); // = vsync
	}

	Engine::~Engine()
	{
		Context::destroy_window(m_mainWindow);
	}

#pragma region static callbacks

	void Engine::opengl_debug_message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam)
	{
		// ignore performance warnings (buffer uses GPU memory, shader recompilation) from nvidia
		if (131185 == id
		 || 131218 == id)
		{
			return; 
		}

		log_debug(">>>> OpenGL DEBUG message callback");

		switch (source) 
		{
			case GL_DEBUG_SOURCE_API:
				log_debug("> Source:   API");
				break;
			case GL_DEBUG_SOURCE_APPLICATION: 
				log_debug("> Source:   APPLICATION");
				break;
			case GL_DEBUG_SOURCE_WINDOW_SYSTEM: 
				log_debug("> Source:   SYSTEM");
				break;
			case GL_DEBUG_SOURCE_SHADER_COMPILER: 
				log_debug("> Source:   COMPILER");
				break;
			case GL_DEBUG_SOURCE_THIRD_PARTY: 
				log_debug("> Source:   PARTY");
				break;
			case GL_DEBUG_SOURCE_OTHER: 
				log_debug("> Source:   OTHER");
				break;
			default: 
				log_debug("> Source:   Unknown");
				break;
		}

		switch (type) 
		{
			case GL_DEBUG_TYPE_ERROR: 
				log_debug("> Type:     ERROR");
				break;
			case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: 
				log_debug("> Type:     DEPRECATED BEHAVIOR");
				break;
			case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: 
				log_debug("> Type:     UNDEFINED BEHAVIOR");
				break;
			case GL_DEBUG_TYPE_PORTABILITY_ARB: 
				log_debug("> Type:     PORTABILITY ARB");
				break;
			case GL_DEBUG_TYPE_PERFORMANCE: 
				log_debug("> Type:     PERFORMANCE");
				break;
			case GL_DEBUG_TYPE_OTHER: 
				log_debug("> Type:     OTHER");
				break;
			default: 
				log_debug("> Type:     Unknown");
				break;
		}

		switch (severity) 
		{
			case GL_DEBUG_SEVERITY_HIGH: 
				log_debug("> Sererity: HIGH");
				break;
			case GL_DEBUG_SEVERITY_MEDIUM: 
				log_debug("> Sererity: MEDIUM");
				break;
			case GL_DEBUG_SEVERITY_LOW: 
				log_debug("> Sererity: LOW");
				break;
			default: 
				log_debug("> Sererity: Unknown");
				break;
		}

		log_debug("> Message:  %s\n", message);
	}

	void Engine::glfw_windowsize_callback(GLFWwindow* window, int width, int height)
	{
		current->m_main_wnd_width = width;
		current->m_main_wnd_height = height;
		current->m_main_wnd_aspectRatio = static_cast<float>(current->m_main_wnd_width) / static_cast<float>(current->m_main_wnd_height);

		for (auto* c : Engine::current->m_windowsize_callbacks)
		{
			(*c)(window, width, height);
		}
	}

	void Engine::glfw_mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
	{
		for (auto& c : Engine::current->m_mouse_button_callbacks)
		{
			(*c)(window, button, action, mods);
		}
	}

	void Engine::glfw_cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
	{
		for (auto& c : Engine::current->m_cursor_pos_callbacks)
		{
			(*c)(window, xpos, ypos);
		}
	}

	void Engine::glfw_scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		for (auto& c : Engine::current->m_scroll_callbacks)
		{
			(*c)(window, xoffset, yoffset);
		}
	}

	void Engine::glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		for (auto& c : Engine::current->m_key_callbacks)
		{
			(*c)(window, key, scancode, action, mods);
		}
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		{
			Engine::current->m_current_scene->Terminate();
		}
	}

	void Engine::glfw_char_callback(GLFWwindow* window, unsigned int codepoint)
	{
		for (auto& c : Engine::current->m_char_callbacks)
		{
			(*c)(window, codepoint);
		}
	}
#pragma endregion

#pragma region event subscriptions handling
	void Engine::SubscribeToWindowsizeCallbacks(std::function<void(GLFWwindow*, int, int)>& fu)
	{
		m_pending_actions.push([this, fuptr{ &fu }]{
			m_windowsize_callbacks.push_back(fuptr);
		});
	}
	void Engine::SubscribeToWindowsizeCallbacks(std::function<void(GLFWwindow*, int, int)>&& fu)
	{
		m_pending_actions.push([this, thefu{ std::move(fu) }]{
			m_stored_windowsize_callbacks.push_back(std::move(thefu));
			m_windowsize_callbacks.push_back(&m_stored_windowsize_callbacks.back());
		});
	}
	void Engine::UnsubscribeFromWindowsizeCallbacks(std::function<void(GLFWwindow*, int, int)>& fu)
	{
		m_pending_actions.push([this, fuptr{ &fu }] {
			m_windowsize_callbacks.erase(
				std::remove(m_windowsize_callbacks.begin(), m_windowsize_callbacks.end(), fuptr),
				m_windowsize_callbacks.end()
			);
		});
	}

	void Engine::SubscribeToMouseButtonCallbacks(std::function<void(GLFWwindow*, int, int, int)>& fu)
	{
		m_pending_actions.push([this, fuptr{ &fu }]{
			m_mouse_button_callbacks.push_back(fuptr);
		});
	}
	void Engine::SubscribeToMouseButtonCallbacks(std::function<void(GLFWwindow*, int, int, int)>&& fu)
	{
		m_pending_actions.push([this, thefu{ std::move(fu) }] {
			m_stored_mouse_button_callbacks.push_back(std::move(thefu));
			m_mouse_button_callbacks.push_back(&m_stored_mouse_button_callbacks.back());
		});
	}
	void Engine::UnsubscribeFromMouseButtonCallbacks(std::function<void(GLFWwindow*, int, int, int)>& fu)
	{
		m_pending_actions.push([this, fuptr{ &fu }]() {
			m_mouse_button_callbacks.erase(
				std::remove(m_mouse_button_callbacks.begin(), m_mouse_button_callbacks.end(), fuptr),
				m_mouse_button_callbacks.end()
			);
		});
	}

	void Engine::SubscribeToCursorPosCallbacks(std::function<void(GLFWwindow*, double, double)>& fu)
	{
		m_pending_actions.push([this, fuptr{ &fu }]() {
			m_cursor_pos_callbacks.push_back(fuptr);
		});
	}
	void Engine::SubscribeToCursorPosCallbacks(std::function<void(GLFWwindow*, double, double)>&& fu)
	{
		m_pending_actions.push([this, thefu{ std::move(fu) }]{
			m_stored_cursor_pos_callbacks.push_back(std::move(thefu));
			m_cursor_pos_callbacks.push_back(&m_stored_cursor_pos_callbacks.back());
		});
	}
	void Engine::UnsubscribeFromCursorPosCallbacks(std::function<void(GLFWwindow*, double, double)>& fu)
	{
		m_pending_actions.push([this, fuptr{ &fu }]() {
			m_cursor_pos_callbacks.erase(
				std::remove(m_cursor_pos_callbacks.begin(), m_cursor_pos_callbacks.end(), fuptr),
				m_cursor_pos_callbacks.end()
			);
		});
	}

	void Engine::SubscribeToScrollCallbacks(std::function<void(GLFWwindow*, double, double)>& fu)
	{
		m_pending_actions.push([this, fuptr{ &fu }]() {
			m_scroll_callbacks.push_back(fuptr);
		});
	}
	void Engine::SubscribeToScrollCallbacks(std::function<void(GLFWwindow*, double, double)>&& fu)
	{
		m_pending_actions.push([this, thefu{ std::move(fu) }]{
			m_stored_scroll_callbacks.push_back(std::move(thefu));
			m_scroll_callbacks.push_back(&m_stored_scroll_callbacks.back());
		});
	}
	void Engine::UnsubscribeFromScrollCallbacks(std::function<void(GLFWwindow*, double, double)>& fu)
	{
		m_pending_actions.push([this, fuptr{ &fu }]() {
			m_scroll_callbacks.erase(
				std::remove(m_scroll_callbacks.begin(), m_scroll_callbacks.end(), fuptr),
				m_scroll_callbacks.end()
			);
		});
	}

	void Engine::SubscribeToKeyCallbacks(std::function<void(GLFWwindow*, int, int, int, int)>& fu)
	{
		m_pending_actions.push([this, fuptr{ &fu }]() {
			m_key_callbacks.push_back(fuptr);
		});
	}
	void Engine::SubscribeToKeyCallbacks(std::function<void(GLFWwindow*, int, int, int, int)>&& fu)
	{
		m_pending_actions.push([this, thefu{ std::move(fu) }]{
			m_stored_key_callbacks.push_back(std::move(thefu)); 
			m_key_callbacks.push_back(&m_stored_key_callbacks.back());
		});
	}
	void Engine::UnsubscribeFromKeyCallbacks(std::function<void(GLFWwindow*, int, int, int, int)>& fu)
	{
		m_pending_actions.push([this, fuptr{ &fu }]() {
			m_key_callbacks.erase(
				std::remove(m_key_callbacks.begin(), m_key_callbacks.end(), fuptr),
				m_key_callbacks.end()
			);
		});
	}

	void Engine::SubscribeToCharCallbacks(std::function<void(GLFWwindow*, unsigned int)>& fu)
	{
		m_pending_actions.push([this, fuptr{ &fu }]() {
			m_char_callbacks.push_back(fuptr);
		});
	}
	void Engine::SubscribeToCharCallbacks(std::function<void(GLFWwindow*, unsigned int)>&& fu)
	{
		m_pending_actions.push([this, thefu{ std::move(fu) }]{
			m_stored_char_callbacks.push_back(std::move(thefu)); 
			m_char_callbacks.push_back(&m_stored_char_callbacks.back());
		});
	}
	void Engine::UnsubscribeFromCharCallbacks(std::function<void(GLFWwindow*, unsigned int)>& fu)
	{
		m_pending_actions.push([this, fuptr{ &fu }]() {
			m_char_callbacks.erase(
				std::remove(m_char_callbacks.begin(), m_char_callbacks.end(), fuptr),
				m_char_callbacks.end()
			);
		});
	}
#pragma endregion

	AntTweakBarManager& Engine::tweak_bar_manager()
	{
		static AntTweakBarManager twbarMgr;
		twbarMgr.enable_tweak_bars();
		return twbarMgr;
	}

	void Engine::BeginFrame()
	{
		while (!m_pending_actions.empty())
		{
			m_pending_actions.front()();
			m_pending_actions.pop();
		}

		ProcessEvents();

		while (!m_pending_actions.empty())
		{
			m_pending_actions.front()();
			m_pending_actions.pop();
		}
	}

	void Engine::EndFrame()
	{
		while (!m_pending_actions.empty())
		{
			m_pending_actions.front()();
			m_pending_actions.pop();
		}

		SwapBuffers();

		auto shouldClose = glfwWindowShouldClose(m_mainWindow);
		if (shouldClose || m_next_scene)
		{
			if (m_current_scene)
			{
				m_current_scene->Terminate();
			}

			if (shouldClose)
			{
				// => Don't load next scene!
				m_next_scene.reset();
			}
		}

		while (!m_pending_actions.empty())
		{
			m_pending_actions.front()();
			m_pending_actions.pop();
		}
	}

	void Engine::SetRootSceneGenFunc(std::function<std::unique_ptr<IScene>()> root_scene_gen_func)
	{
		m_root_scene_generator_func = std::move(root_scene_gen_func);
	}

	void Engine::SetNextScene(std::unique_ptr<IScene> next_scene)
	{
		m_next_scene = std::move(next_scene);
		m_next_is_root = false;
	}

	void Engine::SetRootSceneAsNextScene()
	{
		auto root_scene = m_root_scene_generator_func();
		if (root_scene)
		{
			m_next_scene = std::move(root_scene);
			m_next_is_root = true;
		}
	}

	void Engine::Run()
	{
		if (!m_next_scene)
		{
			SetRootSceneAsNextScene();
		}

		while(m_next_scene)
		{
			m_current_scene.reset();
			m_current_scene = std::move(m_next_scene);
			m_current_is_root = m_next_is_root;
			assert(!m_next_scene);
			m_current_scene->Run();

			if (!m_next_scene && !m_current_is_root)
			{
				SetRootSceneAsNextScene();
			}
		}

		log_debug("--> END OF Application::Run");
	}

	void Engine::StartWithRootScene(std::function<std::unique_ptr<IScene>()> root_scene_gen_func)
	{
		StartWindowedWithRootSceneAndFirstScene(800, 600, std::move(root_scene_gen_func), nullptr);
	}

	void Engine::StartWithFirstScene(std::unique_ptr<IScene> first_scene)
	{
		StartWindowedWithRootSceneAndFirstScene(800, 600, nullptr, std::move(first_scene));
	}

	void Engine::StartWithRootSceneAndFirstScene(std::function<std::unique_ptr<IScene>()> root_scene_gen_func, std::unique_ptr<IScene> first_scene)
	{
		StartWindowedWithRootSceneAndFirstScene(800, 600, std::move(root_scene_gen_func), std::move(first_scene));
	}

	void Engine::StartFullscreenWithRootScene(std::function<std::unique_ptr<IScene>()> root_scene_gen_func)
	{
		StartFullscreenWithRootSceneAndFirstScene(std::move(root_scene_gen_func), nullptr);
	}

	void Engine::StartFullscreenWithFirstScene(std::unique_ptr<IScene> first_scene)
	{
		StartFullscreenWithRootSceneAndFirstScene(nullptr, std::move(first_scene));
	}

	void Engine::StartFullscreenWithRootSceneAndFirstScene(std::function<std::unique_ptr<IScene>()> root_scene_gen_func, std::unique_ptr<IScene> first_scene)
	{
		GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
		if (nullptr == primaryMonitor) {
			log_error("Couldn't get primary monitor");
			std::cout << std::endl << "Press return to terminate..." << std::endl;
			getchar();
		}
		const GLFWvidmode* vidMode = glfwGetVideoMode(primaryMonitor);
		if (nullptr == vidMode) {
			log_error("Couldn't get video mode of primary monitor");
			std::cout << std::endl << "Press return to terminate..." << std::endl;
			getchar();
		}
		StartWithParameters(vidMode->width, vidMode->height, primaryMonitor, std::move(root_scene_gen_func), std::move(first_scene));
	}

	void Engine::StartWindowedWithRootScene(const int width, const int height, std::function<std::unique_ptr<IScene>()> root_scene_gen_func)
	{
		StartWindowedWithRootSceneAndFirstScene(width, height, std::move(root_scene_gen_func), nullptr);
	}

	void Engine::StartWindowedWithFirstScene(const int width, const int height, std::unique_ptr<IScene> first_scene)
	{
		StartWindowedWithRootSceneAndFirstScene(width, height, nullptr, std::move(first_scene));
	}

	void Engine::StartWindowedWithRootSceneAndFirstScene(const int width, const int height, std::function<std::unique_ptr<IScene>()> root_scene_gen_func, std::unique_ptr<IScene> first_scene)
	{
		StartWithParameters(width, height, nullptr, std::move(root_scene_gen_func), std::move(first_scene));
	}

	void Engine::StartWithParameters(const int width, const int height, GLFWmonitor* monitor, std::function<std::unique_ptr<IScene>()> root_scene_gen_func, std::unique_ptr<IScene> first_scene)
	{
		try
		{
			// Initialize context, which will also initialize GLFW
			static e186::Context context;

			// Create the main window
			auto* wnd = context.create_window(width, height, monitor);
			if (!wnd) {
				log_error("Window creation failed");
				std::cout << std::endl << "Press return to terminate..." << std::endl;
				getchar();
			}

			// Create Application instance for the main window, 
			// which also hooks into some GLFW callbacks, and 
			// initializes the GL extension loader library (glad).
			// (has to be static, so that it gets destructed last)
			static e186::Engine app(wnd);
			e186::Engine::current = &app;

			// Set the root scene generation function, if we have one
			if (root_scene_gen_func)
			{
				e186::Engine::current->SetRootSceneGenFunc(std::move(root_scene_gen_func));
			}

			// Set the first scene, if we have one
			if (first_scene)
			{	
				e186::Engine::current->SetNextScene(std::move(first_scene));
			}

			// Let's go! This will load either the first scene or the root scene and run in a loop forever
			// (Or, if no scene was set, it will return immediately and exit the application.)
			e186::Engine::current->Run();
		}
		catch (e186::ExceptionWithCallstack& ecs)
		{
			std::cout << std::endl << "FATAL (uncaught) EXCEPTION with callstack:" << std::endl;
			std::cout << std::endl << ecs.what() << std::endl;
			std::cout << std::endl << ecs.callstack() << std::endl;
			std::cout << std::endl << "Press return to terminate..." << std::endl;
			getchar();
		}
		catch (std::exception& e)
		{
			std::cout << std::endl << "FATAL (uncaught) EXCEPTION:" << std::endl;
			std::cout << std::endl << e.what() << std::endl;
			std::cout << std::endl << "Press return to terminate..." << std::endl;
			getchar();
		}
	}
}
