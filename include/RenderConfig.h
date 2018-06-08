#pragma once

namespace e186
{
	class RenderConfig
	{
	public:
		RenderConfig() 
			: m_vao_handle(0),
			m_render_mode(GL_TRIANGLES),
			m_patch_size(3) {}
		RenderConfig(const VAOType vao, const GLenum mode, const int patch_size = 3) 
			: m_vao_handle(vao),
			m_render_mode(mode),
			m_patch_size(patch_size) {}
		RenderConfig(RenderConfig&&) noexcept = default;
		RenderConfig(const RenderConfig&) noexcept = default;
		RenderConfig& operator=(RenderConfig&&) noexcept = default;
		RenderConfig& operator=(const RenderConfig&) noexcept = default;

		/*! VAO-ID of the data to be rendered */
		VAOType vao_handle() const { return m_vao_handle; }

		/*! Render mode or topology of the data
		*  e.g. GL_TRIANGLES, GL_TRIANGLE_STRIP, etc.
		*  Please refer to https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glDrawElements.xhtml */
		GLenum render_mode() const { return m_render_mode; }

		/*! If render mode is GL_PATCHES, this is the patch size. */
		int patch_size() const { return m_patch_size; }

	private:
		VAOType m_vao_handle;
		GLenum m_render_mode;
		int m_patch_size;
	};

	class MeshRenderConfig : public RenderConfig
	{
	public:
		MeshRenderConfig(MeshRef mesh_ref)
			: m_mesh(std::move(mesh_ref)), RenderConfig()
		{}
		MeshRenderConfig(MeshRef mesh_ref, const VAOType vao, const GLenum mode, const int patch_size = 3)
			: m_mesh(std::move(mesh_ref)), RenderConfig(vao, mode, patch_size)
		{}
		MeshRenderConfig(MeshRef mesh_ref, RenderConfig render_config)
			: m_mesh(std::move(mesh_ref)), RenderConfig(std::move(render_config))
		{}
		MeshRenderConfig(MeshRenderConfig&&) noexcept = default;
		MeshRenderConfig(const MeshRenderConfig&) noexcept = default;
		MeshRenderConfig& operator=(MeshRenderConfig&&) noexcept = default;
		MeshRenderConfig& operator=(const MeshRenderConfig&) noexcept = default;

		Mesh& mesh() const { return m_mesh; }

	private:
		MeshRef m_mesh;
	};
}