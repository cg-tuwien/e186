#pragma once

namespace e186
{
	/*! Holds render configuration for a, e.g., glDrawElements call */
	class RenderConfig
	{
	public:
		RenderConfig() noexcept;
		RenderConfig(const VAOType vao, const GLenum mode, const int patch_size = 3) noexcept;
		RenderConfig(RenderConfig&&) noexcept;
		RenderConfig(const RenderConfig&) noexcept;
		RenderConfig& operator=(RenderConfig&&) noexcept;
		RenderConfig& operator=(const RenderConfig&) noexcept;

		/*! VAO-ID of the data to be rendered */
		VAOType vao_handle() const { return m_vao_handle; }

		/*! Render mode or topology of the data
		*  e.g. GL_TRIANGLES, GL_TRIANGLE_STRIP, etc.
		*  Please refer to https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glDrawElements.xhtml */
		GLenum render_mode() const { return m_render_mode; }

		/*! If render mode is GL_PATCHES, this is the patch size. */
		int patch_size() const { return m_patch_size; }

		bool is_valid() const { return 0 != m_vao_handle; }

	protected:
		VAOType m_vao_handle;
		GLenum m_render_mode;
		int m_patch_size;
	};

	/*! Same render config data as RenderConfig, and a reference to the Mesh in addition. */
	class MeshRenderConfig : public RenderConfig
	{
	public:
		MeshRenderConfig(MeshRef, Shader&) noexcept;
		MeshRenderConfig(MeshRef, Shader&, const VAOType, const GLenum, const int patch_size = 3) noexcept;
		MeshRenderConfig(MeshRef, Shader&, RenderConfig) noexcept;
		MeshRenderConfig(MeshRenderConfig&&) noexcept;
		MeshRenderConfig(const MeshRenderConfig&) noexcept;
		MeshRenderConfig& operator=(MeshRenderConfig&&) noexcept;
		MeshRenderConfig& operator=(const MeshRenderConfig&) noexcept;
		~MeshRenderConfig();

		void ShaderUpdated(Shader& new_shader);

		Mesh& mesh() const { return m_mesh; }
		Shader* shader() const { return m_shader; }

	private:
		void NotifyShaderAboutCreation();
		void NotifyShaderAboutDeletion();

	private:
		MeshRef m_mesh;
		Shader* m_shader;
	};
}