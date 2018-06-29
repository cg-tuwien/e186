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

	void MeshRenderConfig::NotifyShaderAboutCreation()
	{
		if (nullptr != m_shader)
		{
			m_shader->HandleMeshRenderConfigCreated(this);
		}
	}

	void MeshRenderConfig::NotifyShaderAboutDeletion()
	{
		if (nullptr != m_shader)
		{
			m_shader->HandleMeshRenderConfigDeleted(this);
		}
	}

	MeshRenderConfig::MeshRenderConfig(MeshRef mesh_ref, Shader& shader) noexcept
		: RenderConfig(	Mesh::GetOrCreateVAOForShader(mesh_ref, shader), 
						mesh_ref.get().topology(), 
						mesh_ref.get().patch_size()),
		m_mesh(std::move(mesh_ref)),
		m_shader(&shader)
	{
		// inform the shader about the new MeshRenderConfig
		NotifyShaderAboutCreation();
	}

	MeshRenderConfig::MeshRenderConfig(MeshRef mesh_ref, Shader& shader, const VAOType vao, const GLenum mode, const int patch_size) noexcept
		: RenderConfig(vao, mode, patch_size), 
		m_mesh(std::move(mesh_ref)),
		m_shader(&shader)
	{
		// inform the shader about the new MeshRenderConfig
		NotifyShaderAboutCreation();
	}

	MeshRenderConfig::MeshRenderConfig(MeshRef mesh_ref, Shader& shader, RenderConfig render_config) noexcept
		: RenderConfig(std::move(render_config)),
		m_mesh(std::move(mesh_ref)),
		m_shader(&shader)
	{
		// inform the shader about the new MeshRenderConfig
		NotifyShaderAboutCreation();
	}

	MeshRenderConfig::MeshRenderConfig(MeshRenderConfig&& other) noexcept
		: RenderConfig(std::move(other)),
		m_mesh(std::move(other.m_mesh)),
		m_shader(std::move(other.m_shader))
	{
		// cleanup other and invalidate its vao and shader-pointer to indicate it has been moved from
		other.NotifyShaderAboutDeletion();
		other.m_vao_handle = 0;
		other.m_shader = nullptr;
		// inform the shader about the new memory location of the MeshRenderConfig
		NotifyShaderAboutCreation();
	}

	MeshRenderConfig::MeshRenderConfig(const MeshRenderConfig& other) noexcept
		: RenderConfig(other),
		m_mesh(other.m_mesh),
		m_shader(other.m_shader)
	{
		// inform the shader about the new MeshRenderConfig (copy)
		NotifyShaderAboutCreation();
	}

	MeshRenderConfig& MeshRenderConfig::operator=(MeshRenderConfig&& other) noexcept
	{
		// possibly clean myself up
		NotifyShaderAboutDeletion();

		// move data
		RenderConfig::operator=(std::move(other));
		m_mesh = std::move(other.m_mesh);
		m_shader = std::move(other.m_shader);
		
		// Cleanup other and invalidate its vao and shader-pointer to indicate it has been moved from
		other.NotifyShaderAboutDeletion();
		other.m_vao_handle = 0;
		other.m_shader = nullptr;

		// inform the shader about the new memory location of the MeshRenderConfig
		NotifyShaderAboutCreation();
		return *this;
	}

	MeshRenderConfig& MeshRenderConfig::operator=(const MeshRenderConfig& other) noexcept
	{
		// possibly clean myself up
		NotifyShaderAboutDeletion();

		// copy data
		RenderConfig::operator=(other);
		m_mesh = other.m_mesh;
		m_shader = other.m_shader;
		
		// inform the shader about the new memory location of the MeshRenderConfig
		NotifyShaderAboutCreation();
		return *this;
	}
	
	MeshRenderConfig::~MeshRenderConfig()
	{
		NotifyShaderAboutDeletion();
	}

	void MeshRenderConfig::ShaderUpdated(Shader& new_shader)
	{
		*this = std::move(MeshRenderConfig{ m_mesh, new_shader });
	}
}
