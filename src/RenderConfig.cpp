#include "Rect.h"

namespace e186
{
	RenderConfig::RenderConfig() noexcept
		: m_vao_handle(0),
		m_render_mode(GL_TRIANGLES),
		m_patch_size(3)
	{
	}

	RenderConfig::RenderConfig(const VAOType vao, const GLenum mode, const int patch_size) noexcept
		: m_vao_handle(vao),
		m_render_mode(mode),
		m_patch_size(patch_size)
	{
	}

	RenderConfig::RenderConfig(RenderConfig&& other) noexcept
		: m_vao_handle(std::move(other.m_vao_handle)),
		m_render_mode(std::move(other.m_render_mode)),
		m_patch_size(std::move(other.m_patch_size))
	{
		// invalidate other's vao-handle to indicate it has been moved from
		other.m_vao_handle = 0;
	}

	RenderConfig::RenderConfig(const RenderConfig& other) noexcept
		: m_vao_handle(other.m_vao_handle),
		m_render_mode(other.m_render_mode),
		m_patch_size(other.m_patch_size)
	{
	}

	RenderConfig& RenderConfig::operator=(RenderConfig&& other) noexcept
	{
		m_vao_handle = std::move(other.m_vao_handle);
		m_render_mode = std::move(other.m_render_mode);
		m_patch_size = std::move(other.m_patch_size);
		// invalidate other's vao-handle to indicate it has been moved from
		other.m_vao_handle = 0;
		return *this;
	}

	RenderConfig& RenderConfig::operator=(const RenderConfig& other) noexcept
	{
		m_vao_handle = other.m_vao_handle;
		m_render_mode = other.m_render_mode;
		m_patch_size = other.m_patch_size;
		return *this;
	}

	// - - - - - - - - - - - - MeshRenderConfig - - - - - - - - - - - - 

	MeshRenderConfig::MeshRenderConfig(MeshRef mesh_ref, Shader& shader) noexcept
		: RenderConfig(	Mesh::GetOrCreateVAOForShader(mesh_ref, shader), 
						mesh_ref.get().topology(), 
						mesh_ref.get().patch_size()),
		m_mesh(std::move(mesh_ref)),
		m_shader(&shader)
	{
		// inform the shader about the new MeshRenderConfig
		assert(is_valid());
		m_shader->HandleMeshRenderConfigCreated(this);
	}

	MeshRenderConfig::MeshRenderConfig(MeshRef mesh_ref, Shader& shader, const VAOType vao, const GLenum mode, const int patch_size) noexcept
		: RenderConfig(vao, mode, patch_size), 
		m_mesh(std::move(mesh_ref)),
		m_shader(&shader)
	{
		// inform the shader about the new MeshRenderConfig
		assert(is_valid());
		m_shader->HandleMeshRenderConfigCreated(this);
	}

	MeshRenderConfig::MeshRenderConfig(MeshRef mesh_ref, Shader& shader, RenderConfig render_config) noexcept
		: RenderConfig(std::move(render_config)),
		m_mesh(std::move(mesh_ref)),
		m_shader(&shader)
	{
		// inform the shader about the new MeshRenderConfig
		assert(is_valid());
		m_shader->HandleMeshRenderConfigCreated(this);
	}

	MeshRenderConfig::MeshRenderConfig(MeshRenderConfig&& other) noexcept
		: RenderConfig(std::move(other)),
		m_mesh(std::move(other.m_mesh)),
		m_shader(std::move(other.m_shader))
	{
		// invalidate other's vao and shader-pointer to indicate it has been moved from
		other.m_vao_handle = 0;
		other.m_shader = nullptr;
		// inform the shader about the new memory location of the MeshRenderConfig
		assert(is_valid());
		m_shader->HandleMeshRenderConfigMoved(&other, this);
	}

	MeshRenderConfig::MeshRenderConfig(const MeshRenderConfig& other) noexcept
		: RenderConfig(other),
		m_mesh(other.m_mesh),
		m_shader(other.m_shader)
	{
		// inform the shader about the new MeshRenderConfig (copy)
		assert(is_valid());
		m_shader->HandleMeshRenderConfigCreated(this);
	}

	MeshRenderConfig& MeshRenderConfig::operator=(MeshRenderConfig&& other) noexcept
	{
		assert(other.is_valid());
		RenderConfig::operator=(std::move(other));
		m_mesh = std::move(other.m_mesh);
		m_shader = std::move(other.m_shader);
		// invalidate other's vao and shader-pointer to indicate it has been moved from
		other.m_vao_handle = 0;
		other.m_shader = nullptr;
		// inform the shader about the new memory location of the MeshRenderConfig
		assert(is_valid());
		m_shader->HandleMeshRenderConfigMoved(&other, this);
		return *this;
	}

	MeshRenderConfig& MeshRenderConfig::operator=(const MeshRenderConfig& other) noexcept
	{
		auto* shader_at_the_beginning = m_shader;
		assert(other.is_valid());
		RenderConfig::operator=(other);
		m_mesh = other.m_mesh;
		m_shader = other.m_shader;
		// inform shaders, if shaders have changed:
		if (shader_at_the_beginning != m_shader)
		{
			shader_at_the_beginning->HandleMeshRenderConfigDeleted(this);
			m_shader->HandleMeshRenderConfigCreated(this);
		}
		return *this;
	}
	
	MeshRenderConfig::~MeshRenderConfig()
	{
		if (nullptr != m_shader)
		{
			m_shader->HandleMeshRenderConfigDeleted(this);
		}
	}

	void MeshRenderConfig::ShaderUpdated(Shader& new_shader)
	{
		*this = std::move(MeshRenderConfig{ m_mesh, new_shader });
	}
}
