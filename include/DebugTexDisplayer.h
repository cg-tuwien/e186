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
			GLint m_offset_x;
			GLint m_offset_y;
			GLenum m_texture_target;
			GLuint m_gl_handle;
		};

	private:
		TwBar* m_tweak_bar;
		std::vector<DbgTexConfig> m_texture_configs;

		DbgTexConfig* Add(GLsizei width, GLsizei height, GLenum texTarget, GLuint glHandle, std::string name);

	public:
		DebugTexDisplayer();
		~DebugTexDisplayer();

		DbgTexConfig* Add(const TexInfo& data, std::string name = "Texture");
		DbgTexConfig* Add(const Tex2D& data, std::string name = "Texture");
		DbgTexConfig* Add(const FrameBufferObject& data, GLenum attachment, std::string name = "Texture");

		void Render();
	};

}
