#if defined(_WIN32)
    #include "e186stdafx.h"
#endif

#include "VoxelizationTestScene.h"

namespace e186
{
	 

    VoxelizationTestScene::VoxelizationTestScene()
	    : m_termination_requested(false)
	{
	}


	VoxelizationTestScene::~VoxelizationTestScene()
	{
	}

	void VoxelizationTestScene::Terminate()
	{
		m_termination_requested = true;
	}

	void VoxelizationTestScene::Run()
	{
		// LOAD AND VOXELIZE A MODEL

		m_voxelizer = std::unique_ptr<Voxelizer>(new Voxelizer());
		assert(m_voxelizer);

		m_model_for_voxelization = Model::LoadFromFile("assets/models/companion_cube/companion_cube.obj", glm::mat4(1.0f), MOLF_default);
		assert(m_model_for_voxelization);

		m_voxelizer->Voxelize(*m_model_for_voxelization);


		// SETUP SCENE

		// shader to draw scene
		Shader scene_shader;
		scene_shader.AddToMultipleShaderSources(Shader::version_string(), ShaderType::Vertex | ShaderType::Fragment)
			  .AddVertexShaderSourceFromFile("assets/shaders/blinnphong.vert")
			  .AddFragmentShaderSourceFromFile("assets/shaders/blinnphong.frag", { std::make_tuple(0, "oFragColor") })
			  .DeclareAutoMatrix("pMatrix", AutoMatrix::ProjectionMatrix)
			  .DeclareAutoMatrix("vmMatrix", AutoMatrix::ViewMatrix | AutoMatrix::ModelMatrix | AutoMatrix::TransformMatrix)
			  .DeclareAutoMatrix("vmNormalMatrix", AutoMatrix::ViewMatrix | AutoMatrix::ModelMatrix | AutoMatrix::TransformMatrix | AutoMatrix::IsNormalMatrix)
			  .Build();

		// scene models

		m_model_sponza = Model::LoadFromFile("assets/models/sponza/sponza_structure.obj", glm::scale(glm::vec3(.01f, .01f, .01f)), MOLF_default);
		assert(m_model_sponza);
		m_model_sponza->CreateAndUploadGpuData(); // upload vertex pos to vertex array buffer and vertex indices to element array buffer
		auto meshes = m_model_sponza->SelectAllMeshes();
		auto unisetters = Model::CompileUniformSetters(scene_shader, meshes);
		auto render_data = Model::GetOrCreateRenderData(scene_shader, meshes);

		// lights

		AmbientLight ambient_light(glm::vec3(0.1f, 0.1f, 0.1f));
		ambient_light.set_enabled(true);

		DirectionalLight directional_light(glm::vec3(0.25f, 0.25f, 0.25f), glm::vec3(-0.25f, -1.0f, 0.0f));
		directional_light.set_enabled(true);

		PointLight point_light(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 0.0f));
		point_light.set_enabled(false);

		// camera
		QuakeCamera cam;
		cam.set_position(glm::vec3(0.0f, 0.0f, 0.0f));
		cam.LookAlong(glm::vec3(0.0, 0.0f, -1.0f));
		cam.SetPerspectiveProjection(20.0f, Engine::current()->aspect_ratio(), 1.0f, 5000.0f);

		// timer
		MaxFpsTimer timer;

		// GL states before we start
		glDepthMask(GL_TRUE);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);
		glDisable(GL_BLEND);

		while (!m_termination_requested)
		{
			//std::cout << "FRAME BEGIN" << std::endl;
			Engine::current()->BeginFrame();

			// UPDATE

			timer.Tick();

			cam.Update(timer.delta_time());
			auto pM = cam.projection_matrix();
			auto vM = cam.CalculateViewMatrix();

			// DRAW SCENE

			// draw scene to back buffer
			glViewport(0, 0, Engine::current()->window_width(), Engine::current()->window_height());
			glClearColor(0.3f, 0.6f, 1.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			scene_shader.Use();
			scene_shader.SetAutoMatrices(m_model_sponza->transformation_matrix(), glm::mat4(1.0f), vM, pM);
			scene_shader.SetLight("uAmbientLight", ambient_light);
			scene_shader.SetLight("uDirectionalLight", directional_light.GetGpuData(vM));
			scene_shader.SetLight("uPointLight", point_light.GetGpuData(vM));
			RenderMeshesWithAlignedUniformSetters(scene_shader, render_data, unisetters);
			UnbindVAO();

			// DRAW VOXEL GRID

			m_voxelizer->RenderVoxelGrid(vM, pM);
			UnbindVAO();

			Engine::current()->EndFrame();
			//std::cout << "FRAME END" << std::endl;
		}
	}
}
