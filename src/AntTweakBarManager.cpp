#include "AntTweakBarManager.h"

namespace e186
{
	AntTweakBarHandle::AntTweakBarHandle(TwBar* bar) 
		: m_bar(bar)
	{
		assert(m_bar);
	}

	AntTweakBarHandle::AntTweakBarHandle(AntTweakBarHandle&& other) noexcept 
		: m_bar(std::move(other.m_bar))
	{
		other.m_bar = nullptr;
	}

	AntTweakBarHandle& AntTweakBarHandle::operator=(AntTweakBarHandle&& other) noexcept
	{
		this->m_bar = other.m_bar;
		other.m_bar = nullptr;
		return *this;
	}
	
	AntTweakBarHandle::~AntTweakBarHandle()
	{
		if (nullptr != m_bar)
		{
			if (!TwDeleteBar(m_bar))
			{
				auto msg = TwGetLastError();
				if (nullptr != msg)
				{
					log_error(msg);
				}
			}
		}
	}


	AntTweakBarManager::AntTweakBarManager() : m_enabled(false)
	{
	}
	
	AntTweakBarManager::~AntTweakBarManager()
	{
		if (m_enabled)
		{
			Engine::current()->UnsubscribeFromWindowsizeCallbacks(m_window_size_handler);
			Engine::current()->UnsubscribeFromMouseButtonCallbacks(m_mouse_button_handler);
			Engine::current()->UnsubscribeFromCursorPosCallbacks(m_cursor_pos_handler);
			Engine::current()->UnsubscribeFromScrollCallbacks(m_scroll_handler);
			Engine::current()->UnsubscribeFromKeyCallbacks(m_key_handler);
			Engine::current()->UnsubscribeFromCharCallbacks(m_char_handler);

			TwTerminate();
		}
	}

	void AntTweakBarManager::enable_tweak_bars()
	{
		if (!m_enabled)
		{
			TwWindowSize(Engine::current()->window_width(), Engine::current()->window_height());

			m_window_size_handler = [this](GLFWwindow* window, int width, int height)
			{
				if (m_enabled)
				{
					TwWindowSize(width, height);
				}
			};
			Engine::current()->SubscribeToWindowsizeCallbacks(m_window_size_handler);

			m_mouse_button_handler = [this](GLFWwindow* window, int button, int action, int mods)
			{
				if (m_enabled)
				{
					TwEventMouseButtonGLFW(button, action);
				}
			};
			Engine::current()->SubscribeToMouseButtonCallbacks(m_mouse_button_handler);

			m_cursor_pos_handler = [this](GLFWwindow* window, double xpos, double ypos)
			{
				if (m_enabled)
				{
					TwEventMousePosGLFW(static_cast<int>(xpos), static_cast<int>(ypos));
				}
			};
			Engine::current()->SubscribeToCursorPosCallbacks(m_cursor_pos_handler);

			m_scroll_handler = [this](GLFWwindow* window, double xoffset, double yoffset)
			{
				if (m_enabled)
				{
					TwEventMouseWheelGLFW(static_cast<int>(yoffset));
				}
			};
			Engine::current()->SubscribeToScrollCallbacks(m_scroll_handler);

			m_key_handler = [this](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				if (m_enabled)
				{
					TwEventKeyGLFW(key, action);
				}
			};
			Engine::current()->SubscribeToKeyCallbacks(m_key_handler);

			m_char_handler = [this](GLFWwindow* window, unsigned int codepoint)
			{
				TwEventCharGLFW(codepoint, GLFW_PRESS);
			};
			Engine::current()->SubscribeToCharCallbacks(m_char_handler);

			// ========= ANT TWEAK BAR INIT
			TwInit(TW_OPENGL_CORE, nullptr);

			m_enabled = true;
		}
	}

	AntTweakBarHandle AntTweakBarManager::create_new_tweak_bar(const char* name)
	{
		return AntTweakBarHandle{ TwNewBar(name) };
	}

	void AntTweakBarManager::Render()
	{
		TwDraw();
	}
}
