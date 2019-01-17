#pragma once

// note: several header are included by the precompiled prefix header include/e186.h
#include "Tex2D.h"
#include "Tex3D.h"
#include "Tex3dDisplayer.h"
#include "FrameBufferObject.h"



#include "e186.h"

namespace e186
{

	class Voxelizer
	{

	public:

		enum struct VoxelStorageMode
	    {
			Tex3D = 0,
			OctreeHierarchy = 1
	    };

		enum struct FillMode
		{
			None = 0,
			FillEmpty = 1,
			FillInside = 2
		};

	private:
	
		AntTweakBarHandle m_tweak_bar;

		// VOXELIZATION DATA

		VoxelStorageMode m_voxel_storage_mode;

		// the mesh model to be voxelized
		std::unique_ptr<Model> m_model;

		// OpenGL 3D Texture to store voxels in regular grid
		// if RegularGridTex3D enabled, stores result of latest call to Voxelizer::Voxelize()
		Tex3D m_voxels_tex3D;

		// Octree data structure for more efficient voxel storage
		// if OctreeHierarchy enabled, stores result of latest call to Voxelizer::Voxelize()
		//VoxelOctree m_voxels_octree;

		// size of voxel grid in xyz
		unsigned int m_gridSize;

		// scale factor to fit model vertices into viewport (viewport is set to voxel grid size)
		float m_modelScale;

		// enable NVidia hardware conservative rasterization (OpenGL 4.3+), if available
		// GL_NV_conservative_raster is defined by GLAD OpenGL loader in external/glad.h
		// https://developer.nvidia.com/sites/default/files/akamai/opengl/specs/GL_NV_conservative_raster.txt
		bool m_enable_conservative_raster;

		// fill empty voxels
		FillMode m_fill_mode;
		glm::vec3 m_fill_color;

		// VOXELIZATION SHADERS

		// shader for the gpu rasterizer based mesh voxelization
		Shader m_voxelize_shader;

		// compute shader to fill all empty voxels
		Shader m_voxel_fill_empty_shader;

		// compute shader to fill inside of voxelized mesh surface
		Shader m_voxel_fill_inside_shader;

		// VOXEL DISPLAY

		Tex3dDisplayer m_tex3Ddisp;


	public:
	
		Voxelizer();
		~Voxelizer();

		void SetGridSize(uint gridSize); // can also be set via AntTweakBar
		void SetScale(float scale); // can also be set via AntTweakBar

		// voxelize the given mesh model and store the result in a data structure
		// the resulting data structure is determined by m_voxel_storage_mode
		void Voxelize(Model &model);
		void Voxelize(const std::string &modelPath);

		// visualize the voxels
		void RenderVoxelGrid(const glm::mat4 &vM, const glm::mat4 &pM);

	};

}
