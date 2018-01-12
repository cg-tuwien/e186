#pragma once

namespace e186
{
	class AntTweakBarHandle
	{
	public:
		AntTweakBarHandle(TwBar* bar);
		AntTweakBarHandle(const AntTweakBarHandle&) = delete;
		AntTweakBarHandle(AntTweakBarHandle&& other) noexcept;
		AntTweakBarHandle& operator=(const AntTweakBarHandle&) = delete;
		AntTweakBarHandle& operator=(AntTweakBarHandle&& other) noexcept;
		~AntTweakBarHandle();

		operator TwBar*() const { return m_bar; }
		TwBar* get_bar() const { return m_bar; }

	private:
		TwBar* m_bar;
	};

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
		AntTweakBarHandle create_new_tweak_bar(const char* name);
		void Render();
	};

}
