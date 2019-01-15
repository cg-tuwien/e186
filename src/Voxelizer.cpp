#include "Voxelizer.h"

namespace e186
{

    void TW_CALL VoxelizeButtonCallback(void *voxelizerDataVoid);

    Voxelizer::Voxelizer()
	    : m_tweak_bar(Engine::current()->tweak_bar_manager().create_new_tweak_bar("Voxelizer"))
	    , m_voxel_storage_mode(VoxelStorageMode::Tex3D)
	    , m_gridSize(64)
	    , m_modelScale(static_cast<float>(m_gridSize-1))
	    , m_enable_conservative_raster(false)
	    , m_fill_mode(FillMode::None)
	    , m_fill_color(glm::vec3(1))
	    , m_tex3Ddisp(m_voxels_tex3D)
	{

		//std::cout << "Voxelizer Generate Test Data" << std::endl;
		//m_voxels_tex3D.GenerateLDRTestData(m_gridSize, m_gridSize, m_gridSize).Upload().BindAndSetTextureParameters(TexParams::NearestFiltering);

		// BUILD SHADERS

		m_voxelize_shader
		    .AddToMultipleShaderSources(Shader::version_string(), ShaderType::Vertex | ShaderType::Geometry | ShaderType::Fragment)
		    .AddVertexShaderSourceFromFile("assets/shaders/voxelize.vert")
		    .AddGeometryShaderSourceFromFile("assets/shaders/voxelize.geom")
		    .AddFragmentShaderSourceFromFile("assets/shaders/voxelize.frag")
		    .Build();

		m_voxel_fill_empty_shader
		    .AddToMultipleShaderSources(Shader::version_string(), ShaderType::Compute)
		    .AddComputeShaderSourceFromFile("assets/shaders/voxel_fill_empty.comp")
		    .Build();

		m_voxel_fill_inside_shader
		    .AddToMultipleShaderSources(Shader::version_string(), ShaderType::Compute)
		    .AddComputeShaderSourceFromFile("assets/shaders/voxel_fill_inside.comp")
		    .Build();

		// SETUP ANTTWEAKBAR

		TwDefine("'Voxelizer' color='26 27 61' text=light position='40 400' ");

		TwAddVarCB(m_tweak_bar, "Render time (ms)", TW_TYPE_DOUBLE, nullptr, Engine::GetRenderTimeMsCB, Engine::current(), " precision=2 ");

		TwType voxelStorageModeTWType = TwDefineEnumFromString("VoxelStorageMode", "Tex3D,OctreeHierarchy");
		TwAddVarRW(m_tweak_bar, "Voxel Storage Mode", voxelStorageModeTWType, &m_voxel_storage_mode, "");

		TwAddVarRW(m_tweak_bar, "GridSize", TW_TYPE_UINT32, &m_gridSize, "min=0");
		TwAddVarRW(m_tweak_bar, "ModelScale", TW_TYPE_FLOAT, &m_modelScale, "min=0 step=0.1");

#ifdef GL_CONSERVATIVE_RASTERIZATION_NV
		m_enable_conservative_raster = true;
		TwAddVarRW(m_tweak_bar, "NV Conservative Raster", TW_TYPE_BOOLCPP, &m_enable_conservative_raster, nullptr);
#else
		TwAddVarRW(m_tweak_bar, "NV Conservative Raster", TW_TYPE_BOOLCPP, &m_enable_conservative_raster, "readonly=true");
#endif

		TwType fillModeTWType = TwDefineEnumFromString("FillMode", "None,FillEmpty,FillInside");
		TwAddVarRW(m_tweak_bar, "Fill Mode", fillModeTWType, &m_fill_mode, "");
		TwAddVarRW(m_tweak_bar, "Fill Color", TW_TYPE_COLOR3F, glm::value_ptr(m_fill_color), "colormode=hls");

		TwAddButton(m_tweak_bar, "Voxelize!", VoxelizeButtonCallback, this, " label='Voxelize!' ");
	}

	Voxelizer::~Voxelizer()
	{
	}

	void TW_CALL VoxelizeButtonCallback(void *voxelizerDataVoid)
	{
		Voxelizer *voxelizerData = static_cast<Voxelizer *>(voxelizerDataVoid);
		voxelizerData->Voxelize("assets/models/companion_cube/companion_cube.obj");
	}

	void Voxelizer::SetGridSize(uint gridSize)
	{
		m_gridSize = gridSize;
	}

	void Voxelizer::SetScale(float scale)
	{
		m_modelScale = scale;
	}

	void Voxelizer::Voxelize(const std::string &modelPath)
	{
		m_model = Model::LoadFromFile(modelPath, glm::mat4(1.0f), MOLF_default);
		assert(m_model);

		Voxelize(*m_model);
	}

	void Voxelizer::Voxelize(Model& model)
	{
		m_voxels_tex3D.DestroyOnline();
		m_voxels_tex3D.DestroyOffline();
		m_voxels_tex3D.GenerateEmpty(m_gridSize, m_gridSize, m_gridSize).Upload().BindAndSetTextureParameters(TexParams::NearestFiltering);

		// SETUP SHADER DATA

		// predefine axis-aligned orthograhic view-projection matrices for geometry shader
		// note glm::ortho, i.e. the 'classical' orthographic projection matrix just maps to normalized device coords
		// i.e. scales and translates a cube of size of voxel grid to unit cube (no perspective divide needed)
		// the actual 'flattening' is done by the hardware since result is a 2D buffer, z value ends up in zbuffer
		// glm::ortho takes left, right, bottom, top clip planes
		float half_w = m_voxels_tex3D.width()/2.f;
		float half_h = m_voxels_tex3D.height()/2.f;
		float half_d = m_voxels_tex3D.height()/2.f;
		glm::mat4 orthoProjMat = glm::ortho<float>(-half_w, half_w,
		                                           -half_h, half_h,
		                                           0.0f, 2.f * half_d);
		// we use three different with view matrices looking at unit cube in direction X, Y and Z
		glm::mat4 viewMatX = glm::lookAt<float>(glm::vec3(-half_w, 0, 0), // eye
		                                        glm::vec3(0, 0, 0),       // target
		                                        glm::vec3(0, 1, 0));      // up
		glm::mat4 viewMatY = glm::lookAt<float>(glm::vec3(0, -half_h, 0),
		                                        glm::vec3(0, 0, 0),
		                                        glm::vec3(0, 0, 1)); // up vector shouldnt be parallel to eye
		glm::mat4 viewMatZ = glm::lookAt<float>(glm::vec3(0, 0, -half_d),
		                                        glm::vec3(0, 0, 0),
		                                        glm::vec3(0, 1, 0));

		m_voxelize_shader.Use();
		m_voxelize_shader.SetUniform("uScaleFactor", m_modelScale);
		m_voxelize_shader.SetUniform("uViewProjMatOrthoX", orthoProjMat * viewMatX);
		m_voxelize_shader.SetUniform("uViewProjMatOrthoY", orthoProjMat * viewMatY);
		m_voxelize_shader.SetUniform("uViewProjMatOrthoZ", orthoProjMat * viewMatZ);
		m_voxelize_shader.SetUniform("uGridSizeX", static_cast<int>(m_voxels_tex3D.width()));
		m_voxelize_shader.SetUniform("uGridSizeY", static_cast<int>(m_voxels_tex3D.height()));
		m_voxelize_shader.SetUniform("uGridSizeZ", static_cast<int>(m_voxels_tex3D.depth()));
		m_voxelize_shader.SetImageTexture("uVoxelDiffuseColor", m_voxels_tex3D, 0, 0, false, 0, GL_READ_WRITE); // at shader binding 0

		// SETUP MODEL DATA

		model.CreateAndUploadGpuData(); // upload vertex pos to vertex array buffer, indices to element array buffer
		auto meshes = model.SelectAllMeshes();
		auto unisetters = Model::CompileUniformSetters(m_voxelize_shader, meshes);
		auto render_data = Model::GetOrCreateRenderData(m_voxelize_shader, meshes);

		// DRAW (VOXELIZE VIA GPU RASTERIZATION)

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE); // dont write to framebuffer, use image load/store instead
		glDisable(GL_CULL_FACE); // dont discard triangles facing a certain direction
		glDisable(GL_DEPTH_TEST); // dont discard fragments that are behind others

		if (m_enable_conservative_raster) {
			glEnable(GL_CONSERVATIVE_RASTERIZATION_NV);
			std::cout << "GL_CONSERVATIVE_RASTERIZATION_NV enabled: " << (glIsEnabled(GL_CONSERVATIVE_RASTERIZATION_NV) == true ? "yes" : "no") << std::endl;
		}

		// set viewport to size of voxel grid (as many pixels as there are voxels from each unit cube direction
		glViewport(0, 0, m_voxels_tex3D.width(), m_voxels_tex3D.height());

		std::cout << "Voxelizer: Attempting to voxelize" << std::endl;
		RenderMeshesWithAlignedUniformSetters(m_voxelize_shader, render_data, unisetters);
		std::cout << "Voxelizer: Finished voxelization" << std::endl;

		UnbindVAO();

		glDisable(GL_CONSERVATIVE_RASTERIZATION_NV);
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);

		if (m_fill_mode == FillMode::FillEmpty)
		{
			m_voxel_fill_empty_shader.Use();
			m_voxel_fill_empty_shader.SetUniform("uFillColor", m_fill_color);
			m_voxel_fill_empty_shader.SetImageTexture("uTex3D", m_voxels_tex3D, 0, 0, false, 0, GL_READ_WRITE);
			Compute(m_voxel_fill_empty_shader, m_voxels_tex3D.width(), m_voxels_tex3D.height(), m_voxels_tex3D.depth());
			glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_FRAMEBUFFER_BARRIER_BIT);
		}
		else if (m_fill_mode == FillMode::FillInside)
		{
			m_voxel_fill_inside_shader.Use();
			m_voxel_fill_inside_shader.SetUniform("uFillColor", m_fill_color);
			m_voxel_fill_inside_shader.SetImageTexture("uTex3D", m_voxels_tex3D, 0, 0, false, 0, GL_READ_WRITE);
			Compute(m_voxel_fill_inside_shader, m_voxels_tex3D.width(), m_voxels_tex3D.height(), 1);
			glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_FRAMEBUFFER_BARRIER_BIT);
		}

	}

	void Voxelizer::RenderVoxelGrid(const glm::mat4& vM, const glm::mat4& pM)
	{
		m_tex3Ddisp.Render(vM, pM); // use m_tex3Ddisp AntTweakBar ui to adjust transform
	}


}
