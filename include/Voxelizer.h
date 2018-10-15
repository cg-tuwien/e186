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
		

	private:
	
		AntTweakBarHandle m_tweak_bar;

		// VOXELIZATION AND VOXEL DATA

		VoxelStorageMode m_voxel_storage_mode;

		// the mesh model to be voxelized
		std::unique_ptr<Model> m_source_mesh_model;

		// shader for the gpu rasterizer based voxelization
		Shader m_mesh_to_voxel_rasterization_shader;

		int m_voxel_grid_resolution;

		// OpenGL 3D Texture to store voxels in regular grid
		// if RegularGridTex3D enabled, stores result of latest call to Voxelizer::Voxelize()
		Tex3D m_voxels_tex3D;

		Tex3dDisplayer m_tex3Ddisp;

		// enable NVidia hardware conservative rasterization (OpenGL 4.3+), if available
		// GL_NV_conservative_raster is defined by GLAD OpenGL loader in external/glad.h
		// https://developer.nvidia.com/sites/default/files/akamai/opengl/specs/GL_NV_conservative_raster.txt
		bool m_enable_conservative_raster;

		// Octree data structure for more efficient voxel storage
		// if OctreeHierarchy enabled, stores result of latest call to Voxelizer::Voxelize()
		//VoxelOctree m_voxels_octree;


	public:
	
		Voxelizer();
		~Voxelizer();

		// voxelize the given mesh and store the result in a data structure
		// the resulting data structure is determined by m_voxel_storage_mode
		void Voxelize(std::unique_ptr<Model> &sourceMeshModel, int voxelGridResolution);

		// visualize the voxels
		void RenderVoxelGrid(const glm::mat4 &vM, const glm::mat4 &pM);
	};

}
