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
		shader.AddVertexShaderSourceFromFile("assets/shaders/blinnphong.vert")
			.AddFragmentShaderSourceFromFile("assets/shaders/blinnphong.frag", { std::make_tuple(0, "oFragColor") })
			.Build();

		// prepare model:
		m_model->PrepareForShader(shader);

		Tex2D tex;
		tex.FromFile("assets/textures/altitudes.png").Upload(GL_RGB).SetTextureParameters(TexParams_LinearFiltering | TexParams_ClampToEdge);
		check_gl_error("LoadedTextures::altitudes()");

		// create a camera for view and projection matrices:
		QuakeCamera cam;
		cam.set_position(glm::vec3(0.0f, 0.0f, 0.0f));
		cam.LookAlong(glm::vec3(0.0f, 0.0f, -1.0f));
		cam.SetPerspectiveProjection(20.0f, Engine::current->aspect_ratio(), 1.0f, 5000.0f);

		// create a timer
		MaxFpsTimer timer;

		// Set reasonable GL-states before we start
		glDepthMask(GL_TRUE);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);
		glDisable(GL_BLEND);

		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		auto& tM = m_model->transformation_matrix();

		while (!m_termination_requested)
		{
			Engine::current->BeginFrame();
			timer.Tick();

			cam.Update(timer.delta_time());
			auto pM = cam.projection_matrix();
			auto vM = cam.CalculateViewMatrix();

			// render scene to back buffer
			glViewport(0, 0, Engine::current->window_width(), Engine::current->window_height());
			glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			//const auto& pM = cam.projection_matrix();
			//auto vM = cam.CalculateViewMatrix();
			//auto mM = glm::rotate(timer.absolute_timef(), glm::vec3(0.0f, 1.0f, 0.0f));

			auto nM = glm::mat3(vM);

			shader.Use();
			shader.SetSampler("uDiffuseTexSampler", tex);
			shader.SetUniform("normalMatrix", nM);
			shader.SetUniform("vmtMmatrix", vM * tM);
			shader.SetUniform("pvmtMatrix", pM * vM * tM);
			shader.SetUniform("uLightDirection", glm::normalize(nM * glm::vec3(-0.2f, -1.0f, -0.3f)));
			shader.SetUniform("uAmbientLightColor", glm::vec3(0.1f, 0.1f, 0.1f));
			shader.SetUniform("uDiffuseColor", glm::vec3(1.0f, 0.0f, 0.0f));
			shader.SetUniform("uSpecularColor", glm::vec3(0.0f, 1.0f, 0.0f));
			shader.SetUniform("uAmbientColor", glm::vec3(0.1f, 0.1f, 0.3f));
			shader.SetUniform("uEmissiveColor", glm::vec3(0.0f, 0.0f, 0.1f));
			shader.SetUniform("uShininess", 10.0f);

			//m_model->RenderWithShaderGenerateMissingVAO(shader);

			const auto n = m_model->num_meshes();
			for (unsigned int i = 0; i < n; i++)
			{
				if (m_model->mesh_at(i).m_material_data)
				{
					m_model->mesh_at(i).m_material_data->ExecuteUniformSetterForShader(shader);
				}

				m_model->RenderForVertexDataConfigGenerateMissingVAO(i, shader.vertex_attrib_config(), shader.kind_of_primitives());
			}

			Engine::current->EndFrame();
		}
	}
}
