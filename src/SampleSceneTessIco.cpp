#include "e186stdafx.h"
#include "SampleSceneTessIco.h"
#include "MaxFpsTimer.h"
#include "Shader.h"
#include "Model.h"
#include "QuakeCamera.h"

namespace e186
{

	SampleSceneTessIco::SampleSceneTessIco()
		: m_termination_requested(false)
	{
	}


	SampleSceneTessIco::~SampleSceneTessIco()
	{
	}

	void SampleSceneTessIco::Terminate()
	{
		m_termination_requested = true;
	}

	void SampleSceneTessIco::Run()
	{
		// create a shader:
		Shader shader;
		shader.AddVertexShaderSourceFromFile("assets/shaders/geodesic.vert")
		      .AddTesselationControlShaderSourceFromFile("assets/shaders/geodesic.tesc")
		      .AddTesselationEvaluationShaderSourceFromFile("assets/shaders/geodesic.tese")
		      .AddGeometryShaderSourceFromFile("assets/shaders/geodesic.geom")
		      .AddFragmentShaderSourceFromFile("assets/shaders/geodesic.frag", { std::make_tuple(0, "oFragColor") })
		      .Build();

		// load the model:
		auto ico = Model::LoadFromFile("assets/models/icosahedron.obj", glm::mat4(1.0f));
		ico->GenerateVAOsWithVertexAttribConfig(VertexAttribData_Position);

		// create a camera for view and projection matrices:
		QuakeCamera cam;
		cam.set_position(glm::vec3(100.0f, 100.0f, -100.0f));
		cam.LookAt(glm::vec3(0.0f));
		cam.SetPerspectiveProjection(20.0f, Engine::current->aspect_ratio(), 1.0f, 5000.0f);

		// create a timer
		MaxFpsTimer timer;

		// Set reasonable GL-states before we start
		glDepthMask(GL_TRUE);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CW); // but why?
		glDisable(GL_BLEND);

		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		while (!m_termination_requested)
		{
			Engine::current->BeginFrame();
			timer.Tick();

			cam.Update(timer.delta_time());
			auto pM = cam.projection_matrix();
			auto vM = cam.CalculateViewMatrix();
			glm::mat4 mM = glm::scale(glm::vec3(100.0f, 100.0f, 100.0f));

			// render scene to back buffer
			glViewport(0, 0, Engine::current->window_width(), Engine::current->window_height());
			glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			//const auto& pM = cam.projection_matrix();
			//auto vM = cam.CalculateViewMatrix();
			//auto mM = glm::rotate(timer.absolute_timef(), glm::vec3(0.0f, 1.0f, 0.0f));

			shader.Use();
			shader.SetUniform("uInnerTessLevel", 1.0f);
			shader.SetUniform("uOuterTessLevel", 1.0f);
			shader.SetUniform("uProjection", pM);
			shader.SetUniform("uModelview", vM * mM);
			shader.SetUniform("uNormalMatrix", glm::mat3(vM));
			shader.SetUniform("uAmbientMaterial", glm::vec3(0.0f, 1.0f, 0.0f));
			shader.SetUniform("uDiffuseMaterial", glm::vec3(1.0f, 0.0f, 0.0f));
			shader.SetUniform("uLightPosition", glm::vec3(0.0f, 100.0f, -50.0f));

			RenderMesh(shader, ico->mesh_at(0));

			Engine::current->EndFrame();
		}
	}


}
