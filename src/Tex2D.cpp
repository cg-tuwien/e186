#include "Tex2D.h"

#ifdef __linux__
    #define STB_IMAGE_IMPLEMENTATION
    #include "stb_image.h"
#endif

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

	Tex2D::Tex2D(uint8_t* data, GLsizei width, GLsizei height, GLsizei colorChannels, std::function<void(uint8_t*)>&& destroyerFu) :
		TexData(GL_TEXTURE_2D),
		m_width(width),
		m_height(height),
		m_color_channels(colorChannels)
	{
		m_data = data;
		m_destroyer = destroyerFu;
	}

	Tex2D::Tex2D(Tex2D&& other) noexcept :
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

	Tex2D& Tex2D::operator=(Tex2D&& other) noexcept
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

	Tex2D& Tex2D::Generate1pxTexture(uint8_t color_r, uint8_t color_g, uint8_t color_b, GLint image_format)
	{
		auto one_px = new uint8_t[3];
		one_px[0] = color_r;
		one_px[1] = color_g;
		one_px[2] = color_b;
		m_data = one_px;
		m_destroyer = [](uint8_t* data) { delete[] data; };
		m_data_type = GL_UNSIGNED_BYTE;
		m_width = 1;
		m_height = 1;
		m_color_channels = 3;
		m_image_format = image_format;
		return *this;
	}

	Tex2D& Tex2D::FromFile(const std::string& path, bool isHDR)
	{
		int x, y, n;
		stbi_set_flip_vertically_on_load(true);
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
			m_destroyer = [](uint8_t* data) {	stbi_image_free(data); };
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
		uint8_t* flipped_uc;

		auto x = m_width;
		auto y = m_height;
		auto n = m_color_channels;

		size_t cSize;
		uint8_t* orig;
		uint8_t* flip;
		auto isFloat = std::holds_alternative<float*>(m_data);
		if (isFloat)
		{
			flipped_f = new float[x * y * n];
			cSize = sizeof(float);
			orig = reinterpret_cast<uint8_t*>(std::get<float*>(m_data));
			flip = reinterpret_cast<uint8_t*>(flipped_f);
		}
		else
		{
			flipped_uc = new uint8_t[x * y * n];
			cSize = sizeof(uint8_t);
			orig = std::get<uint8_t*>(m_data);
			flip = flipped_uc;
		}

		// Flip!
		for (int i = 0; i < y; ++i)
		{
			memcpy(flip + i * x * n * cSize, //< destination
				   orig + (y - i - 1) * x * n * cSize, //< source
				   x * n * cSize); //< size
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
			m_destroyer = [](uint8_t* data) { delete[] data; };
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
			? reinterpret_cast<uint8_t*>(std::get<float*>(m_data))
			: std::get<uint8_t*>(m_data);
		glTexImage2D(GL_TEXTURE_2D, level, internal_format, m_width, m_height, border, m_image_format, m_data_type, bits);

		check_gl_error("After glTexImage2D in Tex2D::Upload");

		m_gl_handle = gl_texID;
		m_internal_format = internal_format;
		return *this;
	}

	Tex2D& Tex2D::UploadSRGBIfPossible(GLint internal_format,				///< format to store the image in
										GLint border,						///< border-size in px
										GLint level)
	{
		if (-1 == internal_format)
		{
			GLint srgb_format = m_internal_format;
			switch (m_internal_format)
			{
			case GL_BGR:
			case GL_RGB:
				srgb_format = GL_SRGB;
				break;
			case GL_RGB8:
				srgb_format = GL_SRGB8;
				break;
			case GL_RGBA:
			case GL_BGRA:
				srgb_format = GL_SRGB_ALPHA;
				break;
			case GL_RGBA8:
				srgb_format = GL_SRGB8_ALPHA8;
				break;
			}
			Upload(srgb_format, border, level);
		}
		else
		{
			Upload(internal_format, border, level);
		}
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
