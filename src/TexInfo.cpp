#include "TexInfo.h"

namespace e186
{
	TexInfo::TexInfo(GLenum texTarget) :
		m_texture_target(texTarget),
		m_gl_handle(0),
		m_internal_format(TexInfo::kUninitialized),
		m_image_format(TexInfo::kUninitialized),
		m_data_type(0),
		m_border(0)
	{
	}

	TexInfo::TexInfo(GLenum texTarget, GLuint handle, GLint internalFormat, GLint imageFormat, GLenum type, GLint border) :
		m_texture_target(texTarget),
		m_gl_handle(handle),
		m_internal_format(internalFormat),
		m_image_format(imageFormat),
		m_data_type(type),
		m_border(border)
	{
	}

	TexInfo::TexInfo(TexInfo&& other) noexcept :
		// it's all simple data types, so no std::move neccessary here
		m_texture_target(other.m_texture_target),
		m_gl_handle(other.m_gl_handle),
		m_internal_format(other.m_internal_format),
		m_image_format(other.m_image_format),
		m_data_type(other.m_data_type),
		m_border(other.m_border)
	{
		other.m_texture_target = 0;
		other.m_gl_handle = 0;
		other.m_internal_format = -1;
		other.m_image_format = -1;
		other.m_data_type = 0;
		other.m_border = 0;

		log_debug("Move constructing TexInfo with m_gl_handle[%u]", m_gl_handle);
	}

	TexInfo& TexInfo::operator=(TexInfo&& other) noexcept
	{
		// it's all simple data types, so no std::move neccessary here
		m_texture_target = other.m_texture_target;
		m_gl_handle = other.m_gl_handle;
		m_internal_format = other.m_internal_format;
		m_image_format = other.m_image_format;
		m_data_type = other.m_data_type;
		m_border = other.m_border;

		other.m_texture_target = 0;
		other.m_gl_handle = 0;
		other.m_internal_format = -1;
		other.m_image_format = -1;
		other.m_data_type = 0;
		other.m_border = 0;

		log_debug("Move assigning TexInfo with m_gl_handle[%u]", m_gl_handle);
		return *this;
	}

	TexInfo::~TexInfo()
	{
		if (0 != m_gl_handle)
		{
			log_debug("Destructing TexInfo with m_gl_handle[%u]", m_gl_handle);
		}
		DestroyOnline(true);
	}

	TexInfo::operator GLuint() const
	{
		return handle();
	}

	GLenum TexInfo::target() const
	{
		return m_texture_target;
	}

	GLuint TexInfo::handle() const
	{
		return m_gl_handle;
	}

	TexInfo& TexInfo::Bind()
	{
		glBindTexture(target(), handle());
		return *this;
	}

	const TexInfo& TexInfo::Bind() const
	{
		glBindTexture(target(), handle());
		return *this;
	}

	void TexInfo::Unbind() const
	{
		glBindTexture(target(), 0);
	}

	GLint TexInfo::internal_format() const
	{
		return m_internal_format;
	}

	GLint TexInfo::image_format() const
	{
		return m_image_format;
	}

	GLenum TexInfo::data_type() const
	{
		return m_data_type;
	}


	TexInfo& TexInfo::BindAndSetTextureParameters(TexParams parameters)
	{
		Bind();

		if ((TexParams::MirroredRepeat & parameters) != TexParams::None)
		{
			glTexParameteri(m_texture_target, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
			glTexParameteri(m_texture_target, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		}
		else if ((TexParams::Repeat & parameters) != TexParams::None)
		{
			glTexParameteri(m_texture_target, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(m_texture_target, GL_TEXTURE_WRAP_T, GL_REPEAT);
		}
		else if ((TexParams::ClampToEdge & parameters) != TexParams::None)
		{
			glTexParameteri(m_texture_target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(m_texture_target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		} 
		else if ((TexParams::ClampToBorder & parameters) != TexParams::None)
		{
			glTexParameteri(m_texture_target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTexParameteri(m_texture_target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		}

		if ((TexParams::NearestFiltering & parameters) != TexParams::None)
		{	
			glTexParameteri(m_texture_target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		}
		else
		{	
			glTexParameteri(m_texture_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}

		if ((TexParams::EnableTextureCompareMode & parameters) != TexParams::None)
		{
			glTexParameteri(m_texture_target, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
			glTexParameteri(m_texture_target, GL_TEXTURE_COMPARE_FUNC, GL_LESS);
		}

		if ((TexParams::GenerateMipMaps & parameters) != TexParams::None)
		{
			if ((TexParams::TrilinearFiltering & parameters) != TexParams::None)
			{
				glTexParameteri(m_texture_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			}
			else if ((TexParams::AnisotropicFiltering2x & parameters) != TexParams::None)
			{
				glTexParameteri(m_texture_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // not sure if okay
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, 2.0f);
			}
			else if ((TexParams::AnisotropicFiltering4x & parameters) != TexParams::None)
			{
				glTexParameteri(m_texture_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // not sure if okay
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, 4.0f);
			}
			else if ((TexParams::AnisotropicFiltering8x & parameters) != TexParams::None)
			{
				glTexParameteri(m_texture_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // not sure if okay
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, 8.0f);
			}
			else if ((TexParams::AnisotropicFiltering16x & parameters) != TexParams::None)
			{
				glTexParameteri(m_texture_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // not sure if okay
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, 16.0f);
			}
			else if ((TexParams::AnisotropicFilteringMax & parameters) != TexParams::None)
			{
				GLfloat max_anis = 0.0f;
				glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &max_anis);
				glTexParameteri(m_texture_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // not sure if okay
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, max_anis);
			}
			else if ((TexParams::BilinearFiltering & parameters) != TexParams::None)
			{
				glTexParameteri(m_texture_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
			}
			else if ((TexParams::LinearFiltering & parameters) != TexParams::None)
			{
				glTexParameteri(m_texture_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			}
			else
			{	
				glTexParameteri(m_texture_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // trilinear is the default, if nothing specified
			}
		}
		else
		{
			if ((TexParams::NearestFiltering & parameters) != TexParams::None)
			{
				glTexParameteri(m_texture_target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			}
			else
			{
				glTexParameteri(m_texture_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			}
		}

		check_gl_error("after Texture::SetTextureParameters");
		return *this;
	}

	TexInfo& TexInfo::GenerateMipMaps()
	{
		glGenerateMipmap(target());
		return *this;
	}

	const TexInfo& TexInfo::GenerateMipMaps() const
	{
		glGenerateMipmap(target());
		return *this;
	}

	void TexInfo::DestroyOnline(bool is_destructing)
	{
		if (0 == m_gl_handle)
		{
			return;
		}

		glDeleteTextures(1, &m_gl_handle);
		m_gl_handle = 0;
	}

	bool IsFormatSRGB(GLint format)
	{
		return GL_SRGB == format || GL_SRGB8 == format || GL_SRGB_ALPHA == format || GL_SRGB8_ALPHA8 == format;
	}
}
