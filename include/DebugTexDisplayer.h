#pragma once

#include "Tex2D.h"
#include "FrameBufferObject.h"

namespace e186
{

	class DebugTexDisplayer
	{
		static const size_t kVecSize = 128;

	public:
		struct DbgTexConfig
		{
			bool m_enabled;
			bool m_border;
			GLsizei m_width;
			GLsizei m_height;
			float m_scale;
			bool m_scale_relative_to_window_height;
			GLint m_offset_x;
			GLint m_offset_y;
			GLenum m_texture_target;
			GLuint m_gl_handle;
			glm::vec4 m_transform_mul;
			glm::vec4 m_transform_add;
		};

	private:
		AntTweakBarHandle m_tweak_bar;
		std::vector<DbgTexConfig> m_texture_configs;
		std::vector<DbgTexConfig> m_texture_configs_multisample;
		std::unique_ptr<Model> m_quad;
		Shader m_shader;
		Shader m_shader_multisample;
		GLuint m_sampler_loc;
		GLuint m_transform_mul_loc;
		GLuint m_transform_add_loc;
		GLuint m_pvmt_matrix_loc;
		GLuint m_sampler_loc_multisample;
		GLuint m_transform_mul_loc_multisample;
		GLuint m_transform_add_loc_multisample;
		GLuint m_pvmt_matrix_loc_multisample;

		DbgTexConfig* Add(GLsizei width, GLsizei height, GLenum texTarget, GLuint glHandle, std::string name);
		static void RenderConfigs(Shader& shader, std::vector<DbgTexConfig>& configs, const int wnd_height, const glm::mat4& pM, Mesh& quad_mesh, GLuint sampler_loc, GLuint transform_mul_loc, GLuint transform_add_loc, GLuint pvmt_matrix_loc);

	public:
		DebugTexDisplayer();
		~DebugTexDisplayer();

		DbgTexConfig* Add(const TexInfo& data, std::string name = "Texture");
		DbgTexConfig* Add(const Tex2D& data, std::string name = "Texture");
		DbgTexConfig* Add(const FrameBufferObject& data, GLenum attachment, std::string name = "Texture");

		void Render();
	};

}
