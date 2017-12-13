#pragma once

namespace e186
{
	class Tex2D : public TexData
	{
		GLsizei m_width, m_height, m_color_channels;

	public:
		Tex2D();
		Tex2D(unsigned char* data, GLsizei width, GLsizei height, GLsizei colorChannels, std::function<void(unsigned char*)>&& destroyerFu);
		Tex2D(float* data, GLsizei width, GLsizei height, GLsizei colorChannels, std::function<void(float*)>&& destroyerFu);
		Tex2D(const Tex2D& other) = delete;
		Tex2D& operator=(const Tex2D& other) = delete;
		Tex2D(Tex2D&& other);
		Tex2D& operator=(Tex2D&& other);
		virtual ~Tex2D();

		GLsizei width() const;
		GLsizei height() const;
		GLsizei color_channels() const;

		Tex2D& Generate1pxWhite();
		Tex2D& FromFile(const std::string& path, bool isHDR = false);
		Tex2D& Flip();
		Tex2D& Upload(GLint internal_format = -1, GLint border = 0, GLint level = 0);
		Tex2D& DestroyOffline();
		Tex2D& DestroyOnline();

	};
}
