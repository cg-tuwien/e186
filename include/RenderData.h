#pragma once

namespace e186
{
	class Shader;
	class MaterialData;
	
	struct MeshRenderData
	{
		std::vector<MeshRenderConfig> m_mesh_render_configs;
		MeshRenderData() : m_mesh_render_configs{} {}
		auto num_meshes() const { return m_mesh_render_configs.size(); }
	};
}
