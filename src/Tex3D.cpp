#include "Tex3D.h"

namespace e186
{
	Tex3D::Tex3D() :
		TexData(GL_TEXTURE_3D),
		m_width(0),
		m_height(0),
		m_depth(0),
		m_color_channels(0)
	{
	}

	Tex3D::Tex3D(unsigned char* data, GLsizei width, GLsizei height, GLsizei depth, GLsizei colorChannels, std::function<void(unsigned char*)>&& destroyerFu) :
		TexData(GL_TEXTURE_3D),
		m_width(width),
		m_height(height),
		m_depth(depth),
		m_color_channels(colorChannels)
	{
	}

	Tex3D::Tex3D(float* data, GLsizei width, GLsizei height, GLsizei depth, GLsizei colorChannels, std::function<void(float*)>&& destroyerFu) :
		TexData(GL_TEXTURE_3D),
		m_width(width),
		m_height(height),
		m_depth(depth),
		m_color_channels(colorChannels)
	{
	}

	Tex3D::Tex3D(Tex3D&& other) :
		TexData(std::move(other)),
		// it's all simple data types, so no std::move neccessary here
		m_width(other.m_width),
		m_height(other.m_height),
		m_depth(other.m_depth),
		m_color_channels(other.m_color_channels)
	{
		other.m_width = 0;
		other.m_height = 0;
		other.m_depth = 0;
		other.m_color_channels = 0;
	}

	Tex3D& Tex3D::operator=(Tex3D&& other)
	{
		TexData::operator=(std::move(other));
		// it's all simple data types, so no std::move neccessary here
		m_width = other.m_width;
		m_height = other.m_height;
		m_depth = other.m_depth;
		m_color_channels = other.m_color_channels;

		other.m_width = 0;
		other.m_height = 0;
		other.m_depth = 0;
		other.m_color_channels = 0;
		return *this;
	}

	Tex3D::~Tex3D()
	{
	}

	GLsizei Tex3D::width() const
	{
		return m_width;
	}

	GLsizei Tex3D::height() const
	{
		return m_height;
	}

	GLsizei Tex3D::depth() const
	{
		return m_depth;
	}

	GLsizei Tex3D::color_channels() const
	{
		return m_color_channels;
	}

	Tex3D& Tex3D::GenerateEmpty(GLsizei width, GLsizei height, GLsizei depth)
	{
		size_t cc = 4;
		auto ww = static_cast<size_t>(width);
		auto hh = static_cast<size_t>(height);
		auto dd = static_cast<size_t>(depth);
		size_t size = ww * hh * dd * cc;
		float widthf = static_cast<float>(width);
		float heightf = static_cast<float>(height);
		float depthf = static_cast<float>(depth);
		float* data = new float[size];
		for (auto k = 0; k < dd; ++k)
		{
			for (auto j = 0; j < hh; ++j)
			{
				for (auto i = 0; i < ww; ++i)
				{
					for (auto c = 0; c < cc; ++c)
					{
						data[c + i * cc + j * ww * cc + k * hh * ww * cc] = 0.0f;
					}
				}
			}
		}
		m_width = width;
		m_height = height;
		m_depth = depth;
		m_internal_format = GL_RGBA16F;
		m_image_format = GL_RGBA;
		m_data_type = GL_FLOAT;
		m_data = data;
		m_destroyer = [](float* data) { delete[] data; };
		return *this;
	}

	Tex3D& Tex3D::GenerateLDRTestData(GLsizei width, GLsizei height, GLsizei depth)
	{
		size_t cc = 3;
		auto ww = static_cast<size_t>(width);
		auto hh = static_cast<size_t>(height);
		auto dd = static_cast<size_t>(depth);
		size_t size = ww * hh * dd * cc;
		unsigned char* data = new unsigned char[size];
		glm::ivec3 sphere_center(width / 2, height / 2, depth / 2);
		int sphere_radius = glm::min(width, height, depth) / 2;
		for (auto k = 0; k < dd; ++k)
		{
			for (auto j = 0; j < hh; ++j)
			{
				for (auto i = 0; i < ww; ++i)
				{
					auto dist = glm::sqrt(
						glm::pow2(i - sphere_center.x) + glm::pow2(j - sphere_center.y) + glm::pow2(k - sphere_center.z)
					);

					glm::tvec3<unsigned char> tx_color(0, 0, 0);
					if (dist < sphere_radius)
					{
						// is inside sphere
						tx_color.r = static_cast<unsigned char>((255 * i) / ww);
						tx_color.g = static_cast<unsigned char>((255 * j) / hh);
						tx_color.b = static_cast<unsigned char>((255 * k) / dd);
					}
					data[0 + i * cc + j * ww * cc + k * hh * ww * cc] = tx_color.r;
					data[1 + i * cc + j * ww * cc + k * hh * ww * cc] = tx_color.g;
					data[2 + i * cc + j * ww * cc + k * hh * ww * cc] = tx_color.b;
				}
			}
		}
		m_width = width;
		m_height = height;
		m_depth = depth;
		m_internal_format = GL_RGB;
		m_image_format = GL_RGB;
		m_data_type = GL_UNSIGNED_BYTE;
		m_data = data;
		m_destroyer = [](unsigned char* data) { delete[] data; };
		return *this;
	}

	Tex3D& Tex3D::GenerateHDRTestData(GLsizei width, GLsizei height, GLsizei depth)
	{
		size_t cc = 3;
		auto ww = static_cast<size_t>(width);
		auto hh = static_cast<size_t>(height);
		auto dd = static_cast<size_t>(depth);
		size_t size = ww * hh * dd * cc;
		float widthf = static_cast<float>(width);
		float heightf = static_cast<float>(height);
		float depthf = static_cast<float>(depth);
		float* data = new float[size];
		glm::ivec3 sphere_center(width / 2, height / 2, depth / 2);
		int sphere_radius = glm::min(width, height, depth) / 2;
		for (auto k = 0; k < dd; ++k)
		{
			for (auto j = 0; j < hh; ++j)
			{
				for (auto i = 0; i < ww; ++i)
				{
					auto dist = glm::sqrt(
						glm::pow2(i - sphere_center.x) + glm::pow2(j - sphere_center.y) + glm::pow2(k - sphere_center.z)
					);

					glm::vec3 tx_color(0.f, 0.f, 0.f);
					if (dist < sphere_radius)
					{
						// is inside sphere
						tx_color.r = static_cast<unsigned char>(i / ww);
						tx_color.g = static_cast<unsigned char>(j / hh);
						tx_color.b = static_cast<unsigned char>(k / dd);
					}

					data[0 + i * cc + j * ww * cc + k * hh * ww * cc] = tx_color.r;
					data[1 + i * cc + j * ww * cc + k * hh * ww * cc] = tx_color.g;
					data[2 + i * cc + j * ww * cc + k * hh * ww * cc] = tx_color.b;
				}
			}
		}
		m_width = width;
		m_height = height;
		m_depth = depth;
		m_internal_format = GL_RGB16F;
		m_image_format = GL_RGB;
		m_data_type = GL_FLOAT;
		m_data = data;
		m_destroyer = [](float* data) { delete[] data; };
		return *this;
	}

	Tex3D& Tex3D::GenerateDepthTestData(GLsizei width, GLsizei height, GLsizei depth)
	{
		auto ww = static_cast<size_t>(width);
		auto hh = static_cast<size_t>(height);
		auto dd = static_cast<size_t>(depth);
		size_t size = ww * hh * dd;
		float sizef = static_cast<float>(size);
		float* data = new float[size];
		for (auto k = 0; k < dd; ++k)
		{
			for (auto j = 0; j < hh; ++j)
			{
				for (auto i = 0; i < ww; ++i)
				{
					data[i + j * ww + k * hh * ww] = i * j * k / sizef;
				}
			}
		}
		m_width = width;
		m_height = height;
		m_depth = depth;
		m_internal_format = GL_DEPTH_COMPONENT;
		m_image_format = GL_DEPTH_COMPONENT;
		m_data_type = GL_FLOAT;
		m_data = data;
		m_destroyer = [](float* data) { delete[] data; };
		return *this;
	}

	Tex3D& Tex3D::Upload(GLint internal_format, GLint border, GLint level)
	{
		if (-1 == internal_format)
		{
			internal_format = m_internal_format;
		}
			
		GLuint gl_texID;
		// generate an OpenGL texture ID for this texture
		glGenTextures(1, &gl_texID);
		// bind to the new texture ID
		glBindTexture(GL_TEXTURE_3D, gl_texID);

		auto bits = std::holds_alternative<float*>(m_data)
			? reinterpret_cast<unsigned char*>(std::get<float*>(m_data))
			: std::get<unsigned char*>(m_data);
		glTexImage3D(GL_TEXTURE_3D, level, internal_format, m_width, m_height, m_depth, border, m_image_format, m_data_type, bits);

		check_gl_error("After glTexImage3D");

		m_gl_handle = gl_texID;
		m_internal_format = internal_format;
		return *this;
	}

	Tex3D& Tex3D::DestroyOffline()
	{
		if (m_data.index() == std::variant_npos || (GL_FLOAT != m_data_type && GL_UNSIGNED_BYTE != m_data_type && GL_DEPTH_COMPONENT != m_data_type))
		{
			log_warning("No m_data in Tex3D::DestroyOffline()");
			return *this;
		}

		if (GL_FLOAT == m_data_type)
		{
			std::get<std::function<void(float*)>>(m_destroyer)(std::get<float*>(m_data));
			m_data_type = 0;
		}
		else
		{
			std::get<std::function<void(unsigned char*)>>(m_destroyer)(std::get<unsigned char*>(m_data));
			m_data_type = 0;
		}
		return *this;
	}

	Tex3D& Tex3D::DestroyOnline()
	{
		if (0 == m_gl_handle)
		{
			log_warning("GL-Handle is already 0 in Tex3D::DestroyOnline()");
			return *this;
		}

		glDeleteTextures(1, &m_gl_handle);
		m_gl_handle = 0;
		return *this;
	}
}