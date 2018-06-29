#include "Tex2D.h"

#ifdef __linux__
    #define STB_IMAGE_IMPLEMENTATION
    #include "stb_image.h"
#endif

namespace e186
{
	GLsizei Tex2D::CalculateMipMapLevelCount(GLsizei width, GLsizei height)
	{
		return glm::max(glm::log2(width), glm::log2(height)) + 1;
	}

	GLsizei Tex2D::CalculateMipMapLevelCount(glm::tvec2<GLsizei> size)
	{
		return CalculateMipMapLevelCount(size.x, size.y);
	}

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

	GLsizei Tex2D::mipmap_level_count() const
	{
		return CalculateMipMapLevelCount(width(), height());
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

	Tex2D& Tex2D::FromFile(const std::string& path, DynamicRange precision)
	{
		bool isHDR = false;
		switch (precision)
		{
		case DynamicRange::Auto:
			isHDR = stbi_is_hdr(path.c_str());
			break;
		case DynamicRange::ForceLDR:
			isHDR = false;
			break;
		case DynamicRange::ForceHDR:
			isHDR = true;
			break;
		}

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

	Tex2D& Tex2D::TransformValues(uint8_t mul, uint8_t add)
	{
		if (std::holds_alternative<uint8_t*>(m_data))
		{
			auto* data = std::get<uint8_t*>(m_data);
			auto n = m_width * m_height * m_color_channels;
			for (auto i = 0; i < n; ++i)
			{
				data[i] = data[i] * mul + add;
			}
		}
		else
		{
			log_warning("Will not transform Tex2D's float values with uint8_t");
		}
		return *this;
	}

	Tex2D& Tex2D::TransformValues(float mul, float add)
	{
		if (std::holds_alternative<float*>(m_data))
		{
			auto* data = std::get<float*>(m_data);
			auto n = m_width * m_height * m_color_channels;
			for (auto i=0; i < n; ++i)
			{
				data[i] = data[i] * mul + add;
			}
		}
		else
		{
			log_warning("Can not transform Tex2D's uint8_t values with float");
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

	Tex2D& Tex2D::CreateStorage(glm::tvec2<GLsizei> size,			///< storage size width x height
								GLint internal_format,				///< format to store the image in
								GLint levels)						///< number of mipmapping levels
	{
		if (-1 == levels)
		{
			levels = CalculateMipMapLevelCount(size);
		}

		GLuint gl_texID;
		//generate an OpenGL texture ID for this texture
		glGenTextures(1, &gl_texID);
		assert(gl_texID != 0);
		//bind to the new texture ID
		glBindTexture(GL_TEXTURE_2D, gl_texID);

		glTexStorage2D(GL_TEXTURE_2D, levels, internal_format, size.x, size.y);

		m_width = size.x;
		m_height = size.y;
		m_internal_format = internal_format;
		m_image_format = -1;
		m_border = -1;
		m_data_type = 0;
		m_gl_handle = gl_texID;
		return *this;
	}

	Tex2D& Tex2D::CreateView(glm::tvec2<GLsizei> size, const TexInfo& origtexture, GLint internal_format, GLint minlevel, GLint numlevels)
	{
		GLuint gl_texID;
		//generate an OpenGL texture ID for this texture
		glGenTextures(1, &gl_texID);
		assert(gl_texID != 0);

		glTextureView(gl_texID, GL_TEXTURE_2D, origtexture.handle(), internal_format, minlevel, numlevels, 0, 1);

		m_width = size.x;
		m_height = size.y;
		m_internal_format = internal_format;
		m_image_format = -1;
		m_border = -1;
		m_data_type = 0;
		m_gl_handle = gl_texID;
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

	Tex2D& Tex2D::Destroy()
	{
		DestroyOnline();
		DestroyOffline();
		return *this;
	}

	Tex2D::Tex2DInfo Tex2D::GetInfo()
	{
		Tex2DInfo result;
		if (std::holds_alternative<float*>(m_data))
		{
			auto* data = std::get<float*>(m_data);
			result.m_is_hdr = true;
			result.m_min_valueb = 0;
			result.m_max_valueb = 0;
			result.m_min_valuef = data[0];
			result.m_max_valuef = data[0];
			const auto n = m_width * m_height * m_color_channels;
			for (int i = 0; i < n; ++i)
			{
				if (data[i] < result.m_min_valuef) result.m_min_valuef = data[i];
				if (data[i] > result.m_max_valuef) result.m_max_valuef = data[i];
			}
		}
		else
		{
			auto* data = std::get<uint8_t*>(m_data);
			result.m_is_hdr = false;
			result.m_min_valueb = data[0];
			result.m_max_valueb = data[0];
			result.m_min_valuef = 0.0f;
			result.m_max_valuef = 0.0f;
			const auto n = m_width * m_height * m_color_channels;
			for (int i = 0; i < n; ++i)
			{
				if (data[i] < result.m_min_valueb) result.m_min_valueb = data[i];
				if (data[i] > result.m_max_valueb) result.m_max_valueb = data[i];
			}
		}
		return result;
	}
}
