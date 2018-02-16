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
		Shader shader_diffuse;
		shader_diffuse.AddVertexShaderSourceFromFile("assets/shaders/blinnphong.vert")
			.AddFragmentShaderSourceFromFile("assets/shaders/blinnphong.frag", { std::make_tuple(0, "oFragColor") })
			.Build();

		Shader shader_nrm;
		shader_nrm.AddVertexShaderSourceFromFile("assets/shaders/blinnphong_nm.vert")
			.AddFragmentShaderSourceFromFile("assets/shaders/blinnphong_nm.frag", { std::make_tuple(0, "oFragColor") })
			.Build();

		// select some meshes which we intend to render
		auto meshes_diffuse = m_model->SelectMeshes([](const Mesh& mesh)
		{
			return false == static_cast<bool>(mesh.material_data()->height_tex());
		});
		// generate uniform setters for all selected meshes for a specific shader
		auto uniset_diffuse = Model::CompileUniformSetters(shader_diffuse, meshes_diffuse);
		// get VAOs of all selected meshes
		auto vaos_diffuse = Model::GetOrCreateVAOs(shader_diffuse, meshes_diffuse);

		// select some meshes which we intend to render
		auto meshes_nm = m_model->SelectMeshes([](const Mesh& mesh)
		{
			return true == static_cast<bool>(mesh.material_data()->height_tex());
		});
		// generate uniform setters for all selected meshes for a specific shader
		auto uniset_nm = Model::CompileUniformSetters(shader_nrm, meshes_nm);
		// get VAOs of all selected meshes
		auto vaos_nm = Model::GetOrCreateVAOs(shader_nrm, meshes_nm);

		auto fubar = Engine::current()->tweak_bar_manager().create_new_tweak_bar("settings");
		bool whot = false;
		TwAddVarRW(fubar, "umschalten", TW_TYPE_BOOLCPP, &whot, "");

		Tex2D tex;
		tex.FromFile("assets/textures/altitudes.png").UploadSRGBIfPossible().SetTextureParameters(TexParams_LinearFiltering | TexParams_ClampToEdge);
		check_gl_error("LoadedTextures::altitudes()");

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

		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

			//const auto& pM = cam.projection_matrix();
			//auto vM = cam.CalculateViewMatrix();
			//auto mM = glm::rotate(timer.absolute_timef(), glm::vec3(0.0f, 1.0f, 0.0f));

			auto nM = glm::mat3(vM);

			if (whot)
			{
				shader_diffuse.Use();
				shader_diffuse.SetSampler("uDiffuseTexSampler", tex);
				shader_diffuse.SetUniform("normalMatrix", nM);
				shader_diffuse.SetUniform("vmtMmatrix", vM * tM);
				shader_diffuse.SetUniform("pvmtMatrix", pM * vM * tM);
				shader_diffuse.SetUniform("uLightDirection", glm::normalize(nM * glm::vec3(-0.2f, -1.0f, -0.3f)));
				shader_diffuse.SetUniform("uAmbientLightColor", glm::vec3(0.1f, 0.1f, 0.1f));
				shader_diffuse.SetUniform("uDiffuseColor", glm::vec3(1.0f, 0.0f, 0.0f));
				shader_diffuse.SetUniform("uSpecularColor", glm::vec3(0.0f, 1.0f, 0.0f));
				shader_diffuse.SetUniform("uAmbientColor", glm::vec3(0.1f, 0.1f, 0.3f));
				shader_diffuse.SetUniform("uEmissiveColor", glm::vec3(0.0f, 0.0f, 0.1f));
				shader_diffuse.SetUniform("uShininess", 10.0f);
				RenderMeshesWithAlignedUniformSetters(shader_diffuse, vaos_diffuse, uniset_diffuse);
				UnbindVAO();
			}
			else
			{
				shader_nrm.Use();
				shader_nrm.SetSampler("uDiffuseTexSampler", tex);
				shader_nrm.SetSampler("uNormalTexSampler", tex);
				shader_nrm.SetUniform("normalMatrix", nM);
				shader_nrm.SetUniform("vmtMmatrix", vM * tM);
				shader_nrm.SetUniform("pvmtMatrix", pM * vM * tM);
				shader_nrm.SetUniform("uLightDirection", glm::normalize(nM * glm::vec3(-0.2f, -1.0f, -0.3f)));
				shader_nrm.SetUniform("uAmbientLightColor", glm::vec3(0.1f, 0.1f, 0.1f));
				shader_nrm.SetUniform("uDiffuseColor", glm::vec3(1.0f, 0.0f, 0.0f));
				shader_nrm.SetUniform("uSpecularColor", glm::vec3(0.0f, 1.0f, 0.0f));
				shader_nrm.SetUniform("uAmbientColor", glm::vec3(0.1f, 0.1f, 0.3f));
				shader_nrm.SetUniform("uEmissiveColor", glm::vec3(0.0f, 0.0f, 0.1f));
				shader_nrm.SetUniform("uShininess", 10.0f);
				RenderMeshesWithAlignedUniformSetters(shader_nrm, vaos_nm, uniset_nm);
				UnbindVAO();
			}

			Engine::current()->EndFrame();
		}
	}
}
