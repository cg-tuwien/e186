#include "Tex2D.h"

namespace e186
{
	Tex2D::Tex2D() :
		TexData(GL_TEXTURE_2D),
		m_width(0),
		m_height(0),
		m_color_channels(0)
	{
	}

	Tex2D::Tex2D(float* data, GLsizei width, GLsizei height, GLsizei colorChannels, std::function<void(float*)>&& destroyerFu) :
		TexData(GL_TEXTURE_2D),
		m_width(width),
		m_height(height),
		m_color_channels(colorChannels)
	{
		m_data = data;
		m_destroyer = destroyerFu;
	}

	Tex2D::Tex2D(unsigned char* data, GLsizei width, GLsizei height, GLsizei colorChannels, std::function<void(unsigned char*)>&& destroyerFu) :
		TexData(GL_TEXTURE_2D),
		m_width(width),
		m_height(height),
		m_color_channels(colorChannels)
	{
		m_data = data;
		m_destroyer = destroyerFu;
	}

	Tex2D::Tex2D(Tex2D&& other) :
		TexData(std::move(other)),
		// it's all simple data types, so no std::move neccessary here
		m_width(other.m_width),
		m_height(other.m_height),
		m_color_channels(other.m_color_channels)
	{
		other.m_width = 0;
		other.m_height = 0;
		other.m_color_channels = 0;
	}

	Tex2D& Tex2D::operator=(Tex2D&& other)
	{
		TexData::operator=(std::move(other));
		// it's all simple data types, so no std::move neccessary here
		m_width = other.m_width;
		m_height = other.m_height;
		m_color_channels = other.m_color_channels;

		other.m_width = 0;
		other.m_height = 0;
		other.m_color_channels = 0;
		return *this;
	}

	Tex2D::~Tex2D()
	{
	}

	int Tex2D::width() const
	{
		return m_width;
	}

	int Tex2D::height() const
	{
		return m_height;
	}

	int Tex2D::color_channels() const
	{
		return m_color_channels;
	}

	Tex2D& Tex2D::Generate1pxWhite()
	{
		auto one_px = new unsigned char[3];
		one_px[0] = 255;
		one_px[1] = 255;
		one_px[2] = 255;
		m_data = one_px;
		m_destroyer = [](unsigned char* data) { delete[] data; };
		m_data_type = GL_UNSIGNED_BYTE;
		m_width = 1;
		m_height = 1;
		m_color_channels = 3;
		m_image_format = GL_RGB;
		return *this;
	}

	Tex2D& Tex2D::FromFile(const std::string& path, bool isHDR)
	{
		int x, y, n;
		if (isHDR)
		{
			auto data = stbi_loadf(path.c_str(), &x, &y, &n, 0);
			if (nullptr == data)
			{
				log_error("Failed to load HDR image from path [%s]", path.c_str());
				return *this;
			}
			m_data = data;
			m_destroyer = [](float* data) { stbi_image_free(data); };
			m_data_type = GL_FLOAT;	
		}
		else 
		{
			auto data = stbi_load(path.c_str(), &x, &y, &n, 0);
			if (nullptr == data)
			{
				log_error("Failed to load LDR image from path [%s]", path.c_str());
				return *this;
			}
			m_data = data;
			m_destroyer = [](unsigned char* data) {	stbi_image_free(data); };
			m_data_type = GL_UNSIGNED_BYTE;
		}

		m_width = x;
		m_height = y;
		m_color_channels = n;
		if (3 == m_color_channels)
		{
			m_image_format = GL_RGB;
		}
		else if (4 == m_color_channels)
		{
			m_image_format = GL_RGBA;
		}
		else if (1 == m_color_channels)
		{
			m_image_format = GL_RED;
		}
		else if (2 == m_color_channels)
		{
			m_image_format = GL_RG;
		}
		else
		{
			log_warning("Strange color channel count of [%d]", m_color_channels);
		}
		return *this;
	}

	Tex2D& Tex2D::Flip()
	{
		if (m_data.index() == std::variant_npos)
		{
			log_error("No m_date in Flip()");
			return *this;
		}
			
		float* flipped_f;
		unsigned char* flipped_uc;

		auto x = m_width;
		auto y = m_height;
		auto n = m_color_channels;

		size_t cSize;
		unsigned char* orig;
		unsigned char* flip;
		auto isFloat = std::holds_alternative<float*>(m_data);
		if (isFloat)
		{
			flipped_f = new float[x * y * n];
			cSize = sizeof(float);
			orig = reinterpret_cast<unsigned char*>(std::get<float*>(m_data));
			flip = reinterpret_cast<unsigned char*>(flipped_f);
		}
		else
		{
			flipped_uc = new unsigned char[x * y * n];
			cSize = sizeof(unsigned char);
			orig = std::get<unsigned char*>(m_data);
			flip = flipped_uc;
		}

		// Flip!
		for (int i = 0; i < y; ++i)
		{
			memcpy(flip + i * x * n * sizeof(float), orig + (y - i - 1) * x * n * sizeof(float), x * n * sizeof(float));
		}

		DestroyOffline();
		if (isFloat)
		{
			m_data = flipped_f;
			m_destroyer = [](float* data) { delete[] data; };
		}
		else
		{
			m_data = flipped_uc;
			m_destroyer = [](unsigned char* data) { delete[] data; };
		}

		return *this;
	}

	Tex2D& Tex2D::Upload(GLint internal_format,				///< format to store the image in
							GLint border,						///< border-size in px
							GLint level)						///< mipmapping level
	{
		if (-1 == internal_format)
		{
			internal_format = m_image_format;
		}

		GLuint gl_texID;
		//generate an OpenGL texture ID for this texture
		glGenTextures(1, &gl_texID);
		//bind to the new texture ID
		glBindTexture(GL_TEXTURE_2D, gl_texID);

		//store the texture data for OpenGL use
		auto bits = std::holds_alternative<float*>(m_data)
			? reinterpret_cast<unsigned char*>(std::get<float*>(m_data))
			: std::get<unsigned char*>(m_data);
		glTexImage2D(GL_TEXTURE_2D, level, internal_format, m_width, m_height, border, m_image_format, m_data_type, bits);

		check_gl_error("After glTexImage2D in Tex2D::Upload");

		m_gl_handle = gl_texID;
		m_internal_format = internal_format;
		return *this;
	}

	Tex2D& Tex2D::DestroyOffline()
	{
		TexData::DestroyOffline();
		return *this;
	}

	Tex2D& Tex2D::DestroyOnline()
	{
		TexData::DestroyOnline();
		return *this;
	}
}
