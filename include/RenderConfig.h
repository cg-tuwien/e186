#pragma once

namespace e186
{
	struct RenderConfig
	{
		/*! VAO-ID of the data to be rendered */
		VAOType m_vao_handle;

		/*! Render mode or topology of the data 
		 *  e.g. GL_TRIANGLES, GL_TRIANGLE_STRIP, etc.
		 *  Please refer to https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glDrawElements.xhtml */
		GLenum m_render_mode;

		/*! If render mode is GL_PATCHES, this is the patch size. */
		int m_patch_size;

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
	};
}