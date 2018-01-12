#include "SceneSelectorScene.h"

namespace e186
{
	SceneSelectorScene::SceneSelectorScene()
	{
	}


	SceneSelectorScene::~SceneSelectorScene()
	{
	}

	void SceneSelectorScene::Terminate()
	{
		m_termination_requested = true;
	}

	void SceneSelectorScene::Run()
	{
		auto twbar = Engine::current->tweak_bar_manager().create_new_tweak_bar("Scene Selector");
		for (const auto& tup : m_scenes)
		{
			auto& name = std::get<0>(tup);
			const std::function<void()>& func_ref = std::get<1>(tup);

			TwAddButton(twbar, name.c_str(), [](void *clientData) {
				std::function<void()>* func = reinterpret_cast<std::function<void()>*>(clientData);
				(*func)();
			}, 
			reinterpret_cast<void*>(const_cast<std::function<void()>*>(&func_ref)), nullptr);
		}

		while (!m_termination_requested)
		{
			Engine::current->BeginFrame();

			// render scene to back buffer
			glViewport(0, 0, Engine::current->window_width(), Engine::current->window_height());
			glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			Engine::current->EndFrame();
		}
	}
}
