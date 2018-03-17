#include "A1NormalMapping.h"
#include "MaxFpsTimer.h"
#include <random>

namespace artr
{
	A1NormalMapping::A1NormalMapping() :
		m_should_terminate(false),
		m_scene_settings(Engine::current()->tweak_bar_manager().create_new_tweak_bar("Assignment 1 Settings")),
		m_render_gizmos(true),
		m_animate_first_two_point_lights(true),
		m_blinnphong_nm_shader(),
		m_camera(),
		m_ambient_light(glm::vec3(1/255.0f, 2/255.0f, 3/255.0f)),
		m_dir_light(glm::vec3(13/255.0f, 17/255.0f, 27/255.0f), glm::vec3(-0.38f, -0.78f, -0.49f)),
		m_point_lights(),
		m_lights_editor()
	{
		// Load shader sources from files and build a shader which supports normal mapping
		m_blinnphong_nm_shader
			.AddToMultipleShaderSources(Shader::version_string(), ShaderType::Vertex | ShaderType::Fragment)
			.AddVertexShaderSourceFromFile("assets/shaders/blinnphong_nm.vert")
			.AddFragmentShaderSourceFromFile("assets/shaders/blinnphong_nm.frag", { std::make_tuple(0, "oFragColor") })
			.Build();

		// Load the sponza model from file, scale it a bit down, and instruct the model loader 
		// to triangulate it, calculate smooth normals, and also calculate tangents and bitangents.
		m_sponza_model = Model::LoadFromFile("assets/models/sponza/sponza_structure.obj",
			glm::scale(glm::vec3(0.01f)), //< sponza model is in cm => scale to m
			MOLF_triangulate | MOLF_smoothNormals | MOLF_calcTangentSpace);

		// Load the surrounding terrain model from file, scale it a bit down, and instruct the model loader 
		// to triangulate it, calculate smooth normals, and also calculate tangents and bitangents.
		m_terrain_model = Model::LoadFromFile("assets/models/surrounding_terrain/surrounding_terrain.obj",
			glm::translate(glm::vec3(0.0f, -1.0f, 0.0f)) * glm::scale(glm::vec3(0.1f)),
			MOLF_triangulate | MOLF_smoothNormals | MOLF_calcTangentSpace);
		// Create material for terrain manually
		auto terr_mat = std::make_shared<MaterialData>();
		terr_mat->set_ambient_reflectivity(glm::vec3(1.0f, 1.0f, 1.0f));
		terr_mat->set_diffuse_reflectivity(glm::vec3(1.0f, 1.0f, 1.0f));
		terr_mat->set_specular_reflectivity(glm::vec3(0.0f, 0.0f, 0.0f));
		terr_mat->set_emissive_color(glm::vec3(0.0f, 0.0f, 0.0f));
		terr_mat->set_shininess(100.0f);
		terr_mat->set_diffuse_tex(std::make_shared<Tex2D>());
		terr_mat->set_normals_tex(std::make_shared<Tex2D>());
		terr_mat->diffuse_tex()->FromFile("assets/textures/large_metal_debris/large_metal_debris_Base_Color.jpg").UploadSRGBIfPossible().SetTextureParameters(TexParams_AnisotropicFilteringMax | TexParams_Repeat | TexParams_GenerateMipMaps).GenerateMipMaps();
		terr_mat->normals_tex()->FromFile("assets/textures/large_metal_debris/large_metal_debris_Normal.jpg").UploadSRGBIfPossible().SetTextureParameters(TexParams_AnisotropicFilteringMax | TexParams_Repeat | TexParams_GenerateMipMaps).GenerateMipMaps();
		terr_mat->set_tiling(glm::vec2(32.0f, 32.0f));
		m_terrain_model->mesh_at(0).set_material_data(terr_mat);

		// Load a parallelepiped model - use this for the bonus-task
		// TODO: uncomment the following model-loading code for Bonus-Task 1
		/*m_parallelepiped = Model::LoadFromFile("assets/models/parallelepiped.obj",
			glm::rotate(3.1415f / 2.0f, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::scale(glm::vec3(0.7f)),
			MOLF_triangulate | MOLF_smoothNormals | MOLF_calcTangentSpace);*/
		if (m_parallelepiped)
		{
			// If the parallelepiped has been loaded, just assign it sponza's "floor" material (I'm sure, we can find it)
			for (Mesh& mesh : m_sponza_model->SelectAllMeshes())
			{
				if (mesh.material_data()->name().find("floor") != std::string::npos)
				{
					m_parallelepiped->mesh_at(0).set_material_data(mesh.material_data());
				}
			}
		}

		// Initialize the camera
		m_camera.set_position(glm::vec3(-0.67, 0.53, 6.07));
		m_camera.LookAlong(glm::vec3(0.0f, 0.0f, -1.0f));
		m_camera.SetPerspectiveProjection(20.0f, Engine::current()->aspect_ratio(), 0.1f, 1000.0f);

		// Add some settings to the AntTweakBar in order to display and modify stuff
		TwAddVarRO(m_scene_settings, "Render time (ms)", TW_TYPE_UINT32, Engine::current()->renderTime(), nullptr);
		TwAddVarRW(m_scene_settings, "Animate point lights", TW_TYPE_BOOLCPP, &m_animate_first_two_point_lights, nullptr);
		TwAddVarRW(m_scene_settings, "Gizmos on/off", TW_TYPE_BOOLCPP, &m_render_gizmos, nullptr);

		// Add all the lightsources to the LightsourceEditor
		m_lights_editor.Set(&m_ambient_light);
		m_lights_editor.Set(&m_dir_light);
		CreateLotsOfLights();
		for (auto i = 0; i < m_point_lights.size(); ++i)
		{
			m_lights_editor.Add(&m_point_lights.at(i));
		}
	}

	A1NormalMapping::~A1NormalMapping()
	{
	}

	glm::vec3 A1NormalMapping::kInitialPositionOfFirstPointLight(-0.64f, 0.45f, 3.35f);
	glm::vec3 A1NormalMapping::kInitialPositionOfSecondPointLight(-0.05f, 2.12f, 0.53f);

	void A1NormalMapping::CreateLotsOfLights()
	{
		std::vector<glm::vec3> light_colors;
		light_colors.emplace_back(1.0f, 1.0f, 1.0f);
		light_colors.emplace_back(0.878f, 1.000f, 1.000f);
		light_colors.emplace_back(0.957f, 0.643f, 0.376f);
		light_colors.emplace_back(0.000f, 0.000f, 1.000f);
		light_colors.emplace_back(0.251f, 0.878f, 0.816f);
		light_colors.emplace_back(0.000f, 0.980f, 0.604f);
		light_colors.emplace_back(0.545f, 0.000f, 0.545f);
		light_colors.emplace_back(1.000f, 0.000f, 1.000f);
		light_colors.emplace_back(0.984f, 1.000f, 0.729f);
		light_colors.emplace_back(0.780f, 0.082f, 0.522f);
		light_colors.emplace_back(1.000f, 0.843f, 0.000f);
		light_colors.emplace_back(0.863f, 0.078f, 0.235f);
		light_colors.emplace_back(0.902f, 0.902f, 0.980f);
		light_colors.emplace_back(0.678f, 1.000f, 0.184f);

		std::default_random_engine generator;
		generator.seed(186);
		// generates numbers in the range 0..light_colors.size()-1
		std::uniform_int_distribution<size_t> distribution(0, light_colors.size() - 1); 

		// Create a (moving) light near the pillars at the initial view
		m_point_lights.push_back(PointLight(
			light_colors[distribution(generator)],
			kInitialPositionOfFirstPointLight,
			glm::vec4(1.0f, 0.0f, 5.0f, 0.0f)));

		// Create a (moving) light which is especially useful for evaluating Bonus-Task 1
		m_point_lights.push_back(PointLight(
			light_colors[distribution(generator)],
			kInitialPositionOfSecondPointLight,
			glm::vec4(1.0f, 0.0f, 5.0f, 0.0f)));

		{ // Create lots of small lights near the floor
			const auto lb_x = -14.2f;
			const auto lb_z = -6.37f;
			const auto nx = 13;
			const auto nz = 6;
			const auto step_x = (12.93f - lb_x) / (nx - 1);
			const auto step_z = (5.65f - lb_z) / (nz - 1);
			const auto atten = glm::vec4(1.0f, 0.0f, 0.0f, 13.27f);
			for (auto x = 0; x < nx; ++x)
			{
				for (auto z = 0; z < nz; ++z)
				{
					m_point_lights.push_back(PointLight(
						light_colors[distribution(generator)],
						glm::vec3(lb_x + x * step_x, 0.1f, lb_z + z * step_z),
						atten));
				}
			}
		}

		{	// Create several larger lights in the upper floor
			const auto lb_x = -13.36f;
			const auto lb_z = -5.46f;
			const auto nx = 6;
			const auto nz = 3;
			const auto step_x = (12.1f - lb_x) / (nx - 1);
			const auto step_z = (4.84f - lb_z) / (nz - 1);
			const auto atten = glm::vec4(1.0f, 0.0f, 5.666f, 0.0f);
			for (auto x = 0; x < nx; ++x)
			{
				for (auto z = 0; z < nz; ++z)
				{
					m_point_lights.push_back(PointLight(
						light_colors[distribution(generator)],
						glm::vec3(lb_x + x * step_x, 7.0f, lb_z + z * step_z),
						atten));
				}
			}
		}
	}

	void A1NormalMapping::AnimateFirstTwoPointLights(float elapsed_time)
	{
		const auto kSpeedXZ = 0.5f;
		const auto kRadiusXZ = 1.5f;
		m_point_lights[0].set_position(kInitialPositionOfFirstPointLight + glm::vec3(
			kRadiusXZ * glm::sin(kSpeedXZ * elapsed_time), 
			0.0f, 
			kRadiusXZ * glm::cos(kSpeedXZ * elapsed_time)));

		const auto kSpeed = 0.6f;
		const auto kDistanceX = -0.23f;
		const auto kDistanceY = 1.0f;
		m_point_lights[1].set_position(kInitialPositionOfSecondPointLight + glm::vec3(
			kDistanceX * glm::sin(kSpeed * elapsed_time),
			kDistanceY * glm::sin(kSpeed * elapsed_time),
			0.0f));
	}

	void A1NormalMapping::Terminate()
	{
		m_should_terminate = true;
	}

	void A1NormalMapping::Run()
	{
		// First of all, select which meshes to render with which shader:
		//   In the following lines of code, two things are established per mesh:
		//
		//   1. A set of "uniform setters" which is a set of functions that uploads
		//      all the material data to shader uniforms. A shader can declare the intention
		//      to use a specific material property, by specifying the appropriate location
		//      (see MaterialUniformLocation.h for all material data locations available)
		//
		//   2. A Vertex Array Object which contains all the attributes required by the shader.
		//      A shader can declare the intention to use a specific attribute by specifying the
		//      appropriate location (see VertexAttribLocation.h for all attrib-locations available)
		//
		if (m_sponza_model) // 1st MODEL TO RENDER: "SPONZA"
		{
			// Get a list of all sponza-model's meshes which are to be rendered:
			auto sponza_meshes_nm = m_sponza_model->SelectAllMeshes();
			// Prepare so called "uniform setters" for each mesh which upload material data to a shader:
			//    The following method returns a collection of uniform setters for each sub-mesh of the model.
			//    A uniform setter is nothing else than a std::function which calls Shader::SetUniform, 
			//        Shader::SetSampler or similar Shader-class' methods
			//    If a mesh happens to not have a normal map, it will automatically get one assigned 
			//        with normals pointing straight up (w.r.t. tangent space, naturally)
			m_unisetters_sponza = Model::CompileUniformSetters(m_blinnphong_nm_shader, sponza_meshes_nm);
			// Create a VAO for each mesh:
			//  The following method returns a set of VAOs - one for each sub-mesh of the model.
			m_vaos_sponza = Model::GetOrCreateVAOs(m_blinnphong_nm_shader, sponza_meshes_nm);
		}
		if (m_terrain_model) // 2nd MODEL TO RENDER: "TERRAIN"
		{
			// Get a list of all terrain meshes which are to be rendered:
			auto terr_meshes_nm = m_terrain_model->SelectAllMeshes();
			// Prepare uniform setters for the terrain meshes (see description for m_unisetters_sponza above)
			m_unisetters_terr = Model::CompileUniformSetters(m_blinnphong_nm_shader, terr_meshes_nm);
			// Create a VAO for each terrain mesh (see description for m_vaos_sponza above)
			m_vaos_terr = Model::GetOrCreateVAOs(m_blinnphong_nm_shader, terr_meshes_nm);
		}
		if (m_parallelepiped) // 3rd MODEL TO RENDER: "PARALLELEPIPED"
		{
			// You already know the drill...
			auto parallelepiped_meshes_nm = m_parallelepiped->SelectAllMeshes();
			m_unisetters_parallelepiped = Model::CompileUniformSetters(m_blinnphong_nm_shader, parallelepiped_meshes_nm);
			m_vaos_parallelepiped = Model::GetOrCreateVAOs(m_blinnphong_nm_shader, parallelepiped_meshes_nm);
		}

		// Set reasonable GL-states before we start
		glDepthMask(GL_TRUE);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);
		glDisable(GL_BLEND);
		glEnable(GL_MULTISAMPLE);

		// If you want you can activate v-sync
		//Engine::current()->SetVSync(true);

		// Create a timer
		MaxFpsTimer timer;

		// Start the render loop: 
		//   Make sure to call Engine's BeginFrame() and EndFrame() methods in every frame!
		//   Also, make sure to react to Terminate() calls (handled via m_should_terminate here)
		while (!m_should_terminate)
		{
			Engine::current()->BeginFrame();

			// Render scene to back buffer
			glViewport(0, 0, Engine::current()->window_width(), Engine::current()->window_height());
			glClearColor(0.0066f, 0.0135f, 0.024f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// Update timer
			timer.Tick();

			// Update camera and get matrices
			m_camera.Update(timer.delta_time());
			auto pM = m_camera.projection_matrix();
			auto vM = m_camera.CalculateViewMatrix();

			// The first two point lights are moving
			if (m_animate_first_two_point_lights)
				AnimateFirstTwoPointLights(timer.elapsed_timef());

			// Render all the meshes
			Render(pM, vM);

			// Render light source gizmos which indicate the positions and sizes of point lights
			if (m_render_gizmos)
				m_lights_editor.RenderGizmos(m_camera);

			Engine::current()->EndFrame();
		}
	}

	void A1NormalMapping::Render(const glm::mat4& pM, const glm::mat4& vM)
	{
		auto& shader = m_blinnphong_nm_shader;
		shader.Use();

		// Upload all the light data to the shader:
		// (all light data is already passed in the view space to facilitate the transition to view space shading in task 3)
		//    Set AmbientLightData uniform(s):
		//      - uAmbientLight.color
		shader.SetLight("uAmbientLight", m_ambient_light);
		//    Set DirectionalLightData uniform(s):
		//      - uDirectionalLight.direction
		//      - uDirectionalLight.color
		shader.SetLight("uDirectionalLight", m_dir_light.GetGpuData(glm::mat3(vM)));
		//    Set PointLightData uniform(s):
		//      - uPointLight.position
		//      - uPointLight.color
		//      - uPointLight.attenuation
		shader.SetLight("uPointLight", m_point_lights[0].GetGpuData(vM));

		if (m_sponza_model)
		{
			auto mM = m_sponza_model->transformation_matrix();
			SetUniformMatrices(shader, pM, vM, mM);
			RenderMeshesWithAlignedUniformSetters(shader, m_vaos_sponza, m_unisetters_sponza);
		}

		if (m_terrain_model)
		{
			auto mM = m_terrain_model->transformation_matrix();
			SetUniformMatrices(shader, pM, vM, mM);
			RenderMeshesWithAlignedUniformSetters(shader, m_vaos_terr, m_unisetters_terr);
		}

		if (m_parallelepiped) // only relevant for one of the bonus-tasks, will not execute if m_parallelepiped not loaded
		{
			auto mM = m_parallelepiped->transformation_matrix();
			SetUniformMatrices(shader, pM, vM, mM);
			RenderMeshesWithAlignedUniformSetters(shader, m_vaos_parallelepiped, m_unisetters_parallelepiped);
		}

		UnbindVAO();
	}

	void A1NormalMapping::SetUniformMatrices(Shader& shader, const glm::mat4& pM, const glm::mat4& vM, const glm::mat4& mM)
	{
		// compute matrices that combine several space transformations into one
		auto vmM = vM * mM;

		// compute matrices for normal vector transformations
		// remember:
		// 1.) normal vectors have to be transformed with the inverse transpose of the matrix
		// 2.) for orthogonal matrices, the inverse equals the transpose
		//     => the inverse transpose is the transpose transpose, which cancels out
		// 3.) vM is guaranteed orthogonal => it doesn't need inverseTranspose() applied
		//auto vNrmM = glm::mat3(vM);
		//auto mNrmM = glm::inverseTranspose(glm::mat3(mM));
		auto vmNrmM = glm::inverseTranspose(glm::mat3(vmM));

		// Set the uniforms of the shader
		// If a matrix is not used in the shader, nothing happens (except a warning in the console)
		shader.SetUniform("pMatrix", pM);
		shader.SetUniform("vmMatrix", vmM);
		shader.SetUniform("vmNormalMatrix", vmNrmM);
	}
}
