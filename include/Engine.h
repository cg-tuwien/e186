#pragma once

#include "AntTweakBarManager.h"
#include "FileWatcher/FileWatcher.h"
#include "string_utils.h"

namespace e186
{
#pragma region typedefs
	using MeshIdx = int;
	
	using VAOType = GLuint;
	
	using VAOMap = std::unordered_map<VertexAttribData, VAOType>;
	
	class Mesh;
	using MeshRef = std::reference_wrapper<Mesh>;
	
	class MaterialData;
	class Shader;
	using UniformSetter = std::function<void(const Shader&, const MaterialData&)>;
	
	class DirectionalLight;
	using DirLightRef = std::reference_wrapper<DirectionalLight>;
	
	class PointLight;
	using PointLightRef = std::reference_wrapper<PointLight>;
#pragma endregion

	class Engine
	{
		static void APIENTRY opengl_debug_message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam);

		static void glfw_windowsize_callback(GLFWwindow* window, int width, int height);
		static void glfw_mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
		static void glfw_cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);
		static void glfw_scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
		static void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void glfw_char_callback(GLFWwindow* window, unsigned int codepoint);

		std::vector<std::function<void(GLFWwindow*, int, int)>> m_stored_windowsize_callbacks;
		std::vector<std::function<void(GLFWwindow*, int, int)>*> m_windowsize_callbacks;

		std::vector<std::function<void(GLFWwindow*, int, int, int)>> m_stored_mouse_button_callbacks;
		std::vector<std::function<void(GLFWwindow*, int, int, int)>*> m_mouse_button_callbacks;
		
		std::vector<std::function<void(GLFWwindow*, double, double)>> m_stored_cursor_pos_callbacks;
		std::vector<std::function<void(GLFWwindow*, double, double)>*> m_cursor_pos_callbacks;
		
		std::vector<std::function<void(GLFWwindow*, double, double)>> m_stored_scroll_callbacks;
		std::vector<std::function<void(GLFWwindow*, double, double)>*> m_scroll_callbacks;
		
		std::vector<std::function<void(GLFWwindow*, int, int, int, int)>> m_stored_key_callbacks;
		std::vector<std::function<void(GLFWwindow*, int, int, int, int)>*> m_key_callbacks;
		
		std::vector<std::function<void(GLFWwindow*, unsigned int)>> m_stored_char_callbacks;
		std::vector<std::function<void(GLFWwindow*, unsigned int)>*> m_char_callbacks;

#if defined(_DEBUG) && defined(FEATURE_NOT_READY_YET)
		std::vector<std::string> m_files_to_update;
		std::vector<std::tuple<std::vector<std::string>, std::function<void()>*>> m_file_update_callbacks;

		class UpdateListener : public FW::FileWatchListener
		{
			Engine* m_engine; 

		public:
			UpdateListener(Engine* engine);
			void handleFileAction(FW::WatchID watchid, const FW::String& dir, const FW::String& filename, FW::Action action);
		};

		void HandleFileUpdateCallbacks();
	public:
		void NotifyOnFileChanges(std::vector<std::string> files, std::function<void()>* callback);
		void StopFileChangeNotifyCallbacks(std::function<void()>* callback);
#endif

	public:
		std::queue<std::function<void()>> m_pending_actions;

	private:
		static Engine* g_current;
		GLFWwindow* m_mainWindow;
		bool m_running;
		int m_main_wnd_width;
		int m_main_wnd_height;
		float m_main_wnd_aspectRatio;
		bool m_render_tweak_bars;
		unsigned int m_renderTime, m_renderTimerStart;

		std::unique_ptr<IScene> m_current_scene;
		std::unique_ptr<IScene> m_next_scene;
		bool m_next_is_root;
		bool m_current_is_root;
		std::function<std::unique_ptr<IScene>()> m_root_scene_generator_func;

		AntTweakBarManager m_ant_tweak_bar_manager;

#if defined(_DEBUG) && defined(FEATURE_NOT_READY_YET)
		FW::FileWatcher m_file_watcher;
		UpdateListener m_update_listener;
#endif

		void SetRootSceneAsNextScene();

	public:
		Engine(GLFWwindow* mainWindow);
		~Engine();
		Engine(const Engine&) = delete;
		Engine(Engine&&) = delete;
		Engine& operator=(const Engine&) = delete;
		Engine& operator=(Engine&&) = delete;

		static Engine* current() { return Engine::g_current; }

		void SubscribeToWindowsizeCallbacks(std::function<void(GLFWwindow*, int, int)>& fu);
		void SubscribeToWindowsizeCallbacks(std::function<void(GLFWwindow*, int, int)>&& fu);
		void UnsubscribeFromWindowsizeCallbacks(std::function<void(GLFWwindow*, int, int)>& fu);

		void SubscribeToMouseButtonCallbacks(std::function<void(GLFWwindow*, int, int, int)>& fu);
		void SubscribeToMouseButtonCallbacks(std::function<void(GLFWwindow*, int, int, int)>&& fu);
		void UnsubscribeFromMouseButtonCallbacks(std::function<void(GLFWwindow*, int, int, int)>& fu);

		void SubscribeToCursorPosCallbacks(std::function<void(GLFWwindow*, double, double)>& fu);
		void SubscribeToCursorPosCallbacks(std::function<void(GLFWwindow*, double, double)>&& fu);
		void UnsubscribeFromCursorPosCallbacks(std::function<void(GLFWwindow*, double, double)>& fu);

		void SubscribeToScrollCallbacks(std::function<void(GLFWwindow*, double, double)>& fu);
		void SubscribeToScrollCallbacks(std::function<void(GLFWwindow*, double, double)>&& fu);
		void UnsubscribeFromScrollCallbacks(std::function<void(GLFWwindow*, double, double)>& fu);

		void SubscribeToKeyCallbacks(std::function<void(GLFWwindow*, int, int, int, int)>& fu);
		void SubscribeToKeyCallbacks(std::function<void(GLFWwindow*, int, int, int, int)>&& fu);
		void UnsubscribeFromKeyCallbacks(std::function<void(GLFWwindow*, int, int, int, int)>& fu);

		void SubscribeToCharCallbacks(std::function<void(GLFWwindow*, unsigned int)>& fu);
		void SubscribeToCharCallbacks(std::function<void(GLFWwindow*, unsigned int)>&& fu);
		void UnsubscribeFromCharCallbacks(std::function<void(GLFWwindow*, unsigned int)>& fu);

		AntTweakBarManager& tweak_bar_manager();
		
		GLFWwindow* main_window() const
		{
			return m_mainWindow;
		}

		bool running() const
		{
			return m_running;
		}

		// Returns the width of the Application's main main_window
		int window_width() const
		{
			return m_main_wnd_width;
		}

		// Returns the height of the Application's main main_window
		int window_height() const
		{
			return m_main_wnd_height;
		}

		// Return the aspect ratio of the Application's main main_window
		float aspect_ratio() const
		{
			return m_main_wnd_aspectRatio;
		}

		void SwapBuffers() const
		{
			glfwSwapBuffers(m_mainWindow);
		}

		void SetVSync(bool on) const
		{
			glfwSwapInterval(on ? 1 : 0);
		}

		bool IsKeyPressed(int key) const
		{
			return 0 != glfwGetKey(m_mainWindow, key);
		}

		void ProcessEvents() const
		{
			glfwPollEvents();
		}

		/*!	To be called by scenes at the beginning of a frame.
		 *	Processes (input) events.
		 */
		void BeginFrame();

		/*!	To be called by scenes to finish a frame.
		 *	Calls SwapBuffers, which will display the current frame, among other things.
		 */
		void EndFrame();
		
		/*!	Set function which generates the root scene.
		*	Whenever a scene is terminated, the root scene will be opened.
		*	Terminating the root scene will exit the application.
		*/
		void SetRootSceneGenFunc(std::function<std::unique_ptr<IScene>()> root_scene_gen_func);

		/*!	Schedule the next scene to be run, wich will also
		 *	terminate the current scene.
		 */
		void SetNextScene(std::unique_ptr<IScene> next_scene);

		/*! Works off all pending actions
		 */
		void WorkOffPendingActions();

		/*!	Load and run the next scene
		 */
		void Run();

		/*!
		 * Sets a flag which indicates whether or not to render tweak bars
		 */
		void set_render_tweak_bars(bool render_them);

		/*!
		 * Gets a value indicating whether or not to render tweak bars
		 */
		bool render_tweak_bars() const;

		const unsigned int* renderTime();

	public:
		static void StartWithRootScene(std::function<std::unique_ptr<IScene>()> root_scene_gen_func);
		static void StartWithFirstScene(std::function<std::unique_ptr<IScene>()> first_scene_gen_func);
		static void StartWithRootSceneAndFirstScene(std::function<std::unique_ptr<IScene>()> root_scene_gen_func, std::function<std::unique_ptr<IScene>()> first_scene_gen_func);
		static void StartFullscreenWithRootScene(std::function<std::unique_ptr<IScene>()> root_scene_gen_func);
		static void StartFullscreenWithFirstScene(std::function<std::unique_ptr<IScene>()> first_scene_gen_func);
		static void StartFullscreenWithRootSceneAndFirstScene(std::function<std::unique_ptr<IScene>()> root_scene_gen_func, std::function<std::unique_ptr<IScene>()> first_scene_gen_func);
		static void StartWindowedWithRootScene(const int width, const int height, std::function<std::unique_ptr<IScene>()> root_scene_gen_func);
		static void StartWindowedWithFirstScene(const int width, const int height, std::function<std::unique_ptr<IScene>()> first_scene_gen_func);
		static void StartWindowedWithRootSceneAndFirstScene(const int width, const int height, std::function<std::unique_ptr<IScene>()> root_scene_gen_func, std::function<std::unique_ptr<IScene>()> first_scene_gen_func);
		static void StartWithParameters(const int width, const int height, GLFWmonitor* monitor, std::function<std::unique_ptr<IScene>()> root_scene_gen_func, std::function<std::unique_ptr<IScene>()> first_scene_gen_func);
	};

}
