#include "Voxelizer.h"

namespace e186
{

    Voxelizer::Voxelizer()
	    : m_tweak_bar(Engine::current()->tweak_bar_manager().create_new_tweak_bar("Voxelizer"))
	    , m_voxel_storage_mode(VoxelStorageMode::Tex3D)
	    , m_enable_conservative_raster(false)
	    , m_voxel_grid_resolution(128)
	    , m_tex3Ddisp(m_voxels_tex3D)
	{

		std::cout << "Voxelizer::Voxelizer() 3D texture generate test data" << std::endl;
		m_voxels_tex3D.GenerateLDRTestData(128, 128, 128).Upload().BindAndSetTextureParameters(TexParams::NearestFiltering);

		// BUILD SHADER PROGRAMS

		m_mesh_to_voxel_rasterization_shader
			.AddToMultipleShaderSources(Shader::version_string(), ShaderType::Vertex | ShaderType::Geometry | ShaderType::Fragment)
			.AddVertexShaderSourceFromFile("assets/shaders/voxelize.vert")
			.AddGeometryShaderSourceFromFile("assets/shaders/voxelize.geom")
			.AddFragmentShaderSourceFromFile("assets/shaders/voxelize.frag")
			.Build();

		// SETUP ANTTWEAKBAR

		TwDefine("'Voxelizer' color='26 27 61' text=light position='40 400' ");

		TwType voxelStorageModeTWType = TwDefineEnumFromString("VoxelStorageMode", "Tex3D,OctreeHierarchy");
		TwAddVarCB(m_tweak_bar, "Render time (ms)", TW_TYPE_DOUBLE, nullptr, Engine::GetRenderTimeMsCB, Engine::current(), " precision=2 ");
		TwAddVarRW(m_tweak_bar, "Voxel Storage Mode", voxelStorageModeTWType, &m_voxel_storage_mode, "");
#ifdef GL_CONSERVATIVE_RASTERIZATION_NV
		m_enable_conservative_raster = true;
		TwAddVarRW(m_tweak_bar, "NV Conservative Raster", TW_TYPE_BOOLCPP, &m_enable_conservative_raster, nullptr);
#else
		TwAddVarRW(m_tweak_bar, "NV Conservative Raster", TW_TYPE_BOOLCPP, &m_enable_conservative_raster, "readonly=true");
#endif
	}

	Voxelizer::~Voxelizer()
	{
	}

	void Voxelizer::Voxelize(std::unique_ptr<Model>& sourceMeshModel, int voxelGridResolution)
	{
		// TODO IMPLEMENT

		std::cout << "Voxelizer::Voxelize(const Model &sourceMeshModel) not yet fully implemented." << std::endl;

		if (m_enable_conservative_raster) {
			glEnable(GL_CONSERVATIVE_RASTERIZATION_NV);
			std::cout << "GL_CONSERVATIVE_RASTERIZATION_NV enabled: " << (glIsEnabled(GL_CONSERVATIVE_RASTERIZATION_NV) == true ? "yes" : "no") << std::endl;
		}

		m_voxel_grid_resolution = voxelGridResolution;

		// SETUP TARGET DATA STRUCTURES

		m_voxels_tex3D.GenerateEmpty(m_voxel_grid_resolution, m_voxel_grid_resolution, m_voxel_grid_resolution).Upload().BindAndSetTextureParameters(TexParams::NearestFiltering);

		// SETUP SHADER

		// predefine axis-aligned orthograhic view-projection matrices for geometry shader
		// note glm::ortho, i.e. the 'classical' orthographic projection matrix just maps to normalized device coords
		// i.e. scales and translates a defined cube of clipping planes to the unit cube (no perspective divide needed)
		// the actual 'flattening' is done by the hardware since result is a 2D buffer, z value ends up in zbuffer
		// we use three different with view matrices looking at unit cube from the three coordinate axes
		glm::mat4 orthoProjMat = glm::ortho<float>(-1, 1, -1, 1); // left, right, bottom, top clip planes
		glm::mat4 viewProjMatOrthoX = orthoProjMat * glm::lookAt<float>(glm::vec3(2, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); // eye, target, up vector
		glm::mat4 viewProjMatOrthoY = orthoProjMat * glm::lookAt<float>(glm::vec3(0, 2, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, -1)); // up vector shouldnt be parallel to eye
		glm::mat4 viewProjMatOrthoZ = orthoProjMat * glm::lookAt<float>(glm::vec3(0, 0, 2), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

		m_mesh_to_voxel_rasterization_shader.Use();
		m_mesh_to_voxel_rasterization_shader.SetUniform("uViewProjMatOrthoX", viewProjMatOrthoX);
		m_mesh_to_voxel_rasterization_shader.SetUniform("uViewProjMatOrthoY", viewProjMatOrthoY);
		m_mesh_to_voxel_rasterization_shader.SetUniform("uViewProjMatOrthoZ", viewProjMatOrthoZ);
		m_mesh_to_voxel_rasterization_shader.SetUniform("uVoxelGridResolution", static_cast<int>(m_voxel_grid_resolution));
		m_mesh_to_voxel_rasterization_shader.SetImageTexture("uVoxelDiffuseReflectivity", m_voxels_tex3D, 0, 0, false, 0, GL_WRITE_ONLY);
		//m_mesh_to_voxel_rasterization_shader.SetImageTexture("uVoxelNormal", m_voxels_tex3D, 1, 0, false, 0, GL_WRITE_ONLY);


		// select meshes to render
		auto meshes = sourceMeshModel->SelectAllMeshes();
		// generate uniform setters for selected meshes for a specific shader
		auto unisetters = Model::CompileUniformSetters(m_mesh_to_voxel_rasterization_shader, meshes);
		// get VAOs of all selected meshes
		auto render_data = Model::GetOrCreateRenderData(m_mesh_to_voxel_rasterization_shader, meshes);

		// DRAW

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// disable unwanted OpenGL functions
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE); // dont write to framebuffer, we use image load/store instead
		glDisable(GL_CULL_FACE); // we dont want to discard triangles facing a certain direction
		glDisable(GL_DEPTH_TEST); // we dont want to discard fragments that are behind others

		// set the viewport pixel width and height to the size of the voxel grid
		glViewport(0, 0, m_voxel_grid_resolution, m_voxel_grid_resolution);

		//RenderMesh(m_mesh_to_voxel_rasterization_shader, sourceMeshModel->SelectAllMeshes().at(0));

		std::cout << "Voxelizer::Voxelize got so far" << std::endl;

		UnbindVAO();

		glDisable(GL_CONSERVATIVE_RASTERIZATION_NV);
	}

	void Voxelizer::RenderVoxelGrid(const glm::mat4& vM, const glm::mat4& pM)
	{

		m_tex3Ddisp.Render(glm::scale(glm::vec3(0.1f, 0.1f, 0.1f)), vM, pM);

	}


}
