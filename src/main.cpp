// entry point for the console application.

#ifdef  _WIN32
    #include "stdafx.h"
#endif
#include "SceneSelectorScene.h"
#include "SampleSceneTessIco.h"
#include "ModelExplorerScene.h"
#include "../assignment1/A1NormalMapping.h"

int main()
{
	e186::Engine::StartWindowedWithRootScene(1600, 900, []()
	{
		auto sel_scene = std::make_unique<e186::SceneSelectorScene>();
		sel_scene->AddScene<e186::SampleSceneTessIco>();
		sel_scene->AddSceneGenFunc("Model Explorer: Sponza", []() { return std::make_unique<e186::ModelExplorerScene>("assets/models/sponza/sponza_structure.obj", glm::scale(glm::vec3(.01f, .01f, .01f))); });
		sel_scene->AddSceneGenFunc("Assignment1", []() { return std::make_unique<artr::A1NormalMapping>(); });
		return sel_scene;
	});
}

