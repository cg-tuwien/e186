#include "e186stdafx.h"
#include "ModelExplorerScene.h"
#include "MaxFpsTimer.h"
#include "QuakeCamera.h"
#include <glad/glad.h>
#include "Stopwatch.h"

namespace e186
{


	ModelExplorerScene::ModelExplorerScene(std::string model_to_load_path, glm::mat4 transformation_matrix, unsigned int model_loader_flags) : m_termination_requested(false)
	{
		m_model = Model::LoadFromFile(model_to_load_path, transformation_matrix, model_loader_flags);
	}


	ModelExplorerScene::~ModelExplorerScene()
	{
	}

	void ModelExplorerScene::Terminate()
	{
		m_termination_requested = true;
	}

	void ModelExplorerScene::Run()
	{
		// create a shader:
		Shader shader;
		shader.AddToMultipleShaderSources(Shader::version_string(), ShaderType::Vertex | ShaderType::Fragment)
			.AddVertexShaderSourceFromFile("assets/shaders/blinnphong.vert")
			.AddFragmentShaderSourceFromFile("assets/shaders/blinnphong.frag", { std::make_tuple(0, "oFragColor") })
			.DeclareAutoMatrix("pMatrix", AutoMatrix::ProjectionMatrix)
			.DeclareAutoMatrix("vmMatrix", AutoMatrix::ViewMatrix | AutoMatrix::ModelMatrix | AutoMatrix::TransformMatrix)
			.DeclareAutoMatrix("vmNormalMatrix", AutoMatrix::ViewMatrix | AutoMatrix::ModelMatrix | AutoMatrix::TransformMatrix | AutoMatrix::IsNormalMatrix)
			.Build();

		// select some meshes which we intend to render
		auto meshes = m_model->SelectAllMeshes();
		// generate uniform setters for all selected meshes for a specific shader
		auto unisetters = Model::CompileUniformSetters(shader, meshes);
		// get VAOs of all selected meshes
		auto vaos = Model::GetOrCreateVAOs(shader, meshes);

		AmbientLight ambient_light(glm::vec3(0.1f, 0.1f, 0.1f));
		ambient_light.set_enabled(true);

		DirectionalLight directional_light(glm::vec3(0.25f, 0.25f, 0.25f), glm::vec3(-0.25f, -1.0f, 0.0f));
		directional_light.set_enabled(true);

		PointLight point_light(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 0.0f));
		point_light.set_enabled(false);

		// create a camera for view and projection matrices:
		QuakeCamera cam;
		cam.set_position(glm::vec3(0.0f, 0.0f, 0.0f));
		cam.LookAlong(glm::vec3(0.0f, 0.0f, -1.0f));
		cam.SetPerspectiveProjection(20.0f, Engine::current()->aspect_ratio(), 1.0f, 5000.0f);

		// create a timer
		MaxFpsTimer timer;

		// Set reasonable GL-states before we start
		glDepthMask(GL_TRUE);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);
		glDisable(GL_BLEND);

		auto& tM = m_model->transformation_matrix();

		while (!m_termination_requested)
		{
			Engine::current()->BeginFrame();
			timer.Tick();

			cam.Update(timer.delta_time());
			auto pM = cam.projection_matrix();
			auto vM = cam.CalculateViewMatrix();

			// render scene to back buffer
			glViewport(0, 0, Engine::current()->window_width(), Engine::current()->window_height());
			glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			shader.Use();
			shader.SetAutoMatrices(m_model->transformation_matrix(), glm::mat4(1.0f), vM, pM);
			shader.SetLight("uAmbientLight", ambient_light);
			shader.SetLight("uDirectionalLight", directional_light.GetGpuData(vM));
			shader.SetLight("uPointLight", point_light.GetGpuData(vM));
			RenderMeshesWithAlignedUniformSetters(shader, vaos, unisetters);
			UnbindVAO();

			Engine::current()->EndFrame();
		}
	}
}
