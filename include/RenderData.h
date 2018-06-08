#pragma once

namespace e186
{
	class Shader;
	class MaterialData;
	
	struct MeshRenderData
	{
		VertexAttribData m_vertex_attrib_config;
		std::vector<MeshRenderConfig> m_mesh_render_configs;
		MeshRenderData() : m_vertex_attrib_config{ VertexAttribData::Nothing }, m_mesh_render_configs{} {}
		auto num_meshes() const { return m_mesh_render_configs.size(); }
	};
}
