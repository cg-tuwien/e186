#pragma once

namespace e186
{
	class Tex3D : public TexData
	{
		GLsizei m_width, m_height, m_depth, m_color_channels;

	public:
		Tex3D();
		Tex3D(unsigned char* data, GLsizei width, GLsizei height, GLsizei depth, GLsizei colorChannels, std::function<void(unsigned char*)>&& destroyerFu);
		Tex3D(float* data, GLsizei width, GLsizei height, GLsizei depth, GLsizei colorChannels, std::function<void(float*)>&& destroyerFu);
		Tex3D(const Tex3D& other) = delete;
		Tex3D& operator=(const Tex3D& other) = delete;
		Tex3D(Tex3D&& other);
		Tex3D& operator=(Tex3D&& other);
		~Tex3D();

		GLsizei width() const;
		GLsizei height() const;
		GLsizei depth() const;
		GLsizei color_channels() const;

		Tex3D& GenerateEmpty(GLsizei width, GLsizei height, GLsizei depth);
		Tex3D& GenerateLDRTestData(GLsizei width, GLsizei height, GLsizei depth);
		Tex3D& GenerateHDRTestData(GLsizei width, GLsizei height, GLsizei depth);
		Tex3D& GenerateDepthTestData(GLsizei width, GLsizei height, GLsizei depth);
		Tex3D& Upload(GLint internal_format = -1, GLint border = 0, GLint level = 0);
		Tex3D& DestroyOffline();
		Tex3D& DestroyOnline();
	};
}
