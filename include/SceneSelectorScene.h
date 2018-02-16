#pragma once
namespace e186
{
	class SceneSelectorScene : public IScene
	{
	public:
		SceneSelectorScene();
		virtual ~SceneSelectorScene();
		void Terminate() override;
		void Run() override;

		template <typename SceneType>
		void AddScene(std::string scene_name = "")
		{
			m_scenes.emplace_back(scene_name.empty() ? typeid(SceneType).name() : scene_name, []() {
				Engine::current()->SetNextScene(std::make_unique<SceneType>());
			});
		}

		template <typename GenFuncT>
		void AddSceneGenFunc(std::string scene_name, GenFuncT gen_func)
		{
			m_scenes.emplace_back(scene_name, [gen_func = std::move(gen_func)]() {
				Engine::current()->SetNextScene(gen_func());
			});
		}

	private:
		bool m_termination_requested;
		std::vector<std::tuple<std::string, std::function<void()>>> m_scenes;
		std::function<void(GLFWwindow*, int, int, int, int)> m_key_handler;
	};
}
