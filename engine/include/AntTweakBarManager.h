#pragma once

namespace e186
{

	class AntTweakBarManager
	{
		bool m_enabled;
		std::function<void(GLFWwindow*, int, int)> m_window_size_handler;
		std::function<void(GLFWwindow*, int, int, int)> m_mouse_button_handler;
		std::function<void(GLFWwindow*, double, double)> m_cursor_pos_handler;
		std::function<void(GLFWwindow*, double, double)> m_scroll_handler;
		std::function<void(GLFWwindow*, int, int, int, int)> m_key_handler;
		std::function<void(GLFWwindow*, unsigned int)> m_char_handler;

	public:
		AntTweakBarManager();
		AntTweakBarManager(const AntTweakBarManager& other) = delete;
		AntTweakBarManager& operator=(const AntTweakBarManager& other) = delete;
		AntTweakBarManager(AntTweakBarManager&& other) = delete;
		AntTweakBarManager& operator=(AntTweakBarManager&& other) = delete;
		~AntTweakBarManager();

		void enable_tweak_bars();
		TwBar* create_new_tweak_bar(const char* name);
		void destroy_tweak_bar(TwBar* bar);
		void Render();
	};

}
