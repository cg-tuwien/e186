#pragma once

namespace e186
{

	class Tex2D : public TexData
	{
	public:
		enum struct DynamicRange
		{
			Auto,
			ForceLDR,
			ForceHDR
		};

		struct Tex2DInfo
		{
			bool m_is_hdr;
			uint8_t m_min_valueb;
			uint8_t m_max_valueb;
			float m_min_valuef;
			float m_max_valuef;
		};

		static GLsizei CalculateMipMapLevelCount(GLsizei width, GLsizei height);
		static GLsizei CalculateMipMapLevelCount(glm::tvec2<GLsizei> size);

		Tex2D();
		Tex2D(uint8_t* data, GLsizei width, GLsizei height, GLsizei colorChannels, std::function<void(uint8_t*)>&& destroyer_func);
		Tex2D(float* data, GLsizei width, GLsizei height, GLsizei colorChannels, std::function<void(float*)>&& destroyer_func);
		Tex2D(const Tex2D& other) = delete;
		Tex2D& operator=(const Tex2D& other) = delete;
		Tex2D(Tex2D&& other) noexcept;
		Tex2D& operator=(Tex2D&& other) noexcept;
		virtual ~Tex2D();

		GLsizei width() const;
		GLsizei height() const;
		GLsizei color_channels() const;
		GLsizei mipmap_level_count() const;

		Tex2D& Generate1pxTexture(uint8_t color_r, uint8_t color_g, uint8_t color_b, GLint image_format = GL_RGB);
		Tex2D& FromFile(const std::string& path, DynamicRange precision = DynamicRange::Auto);
		Tex2D& TransformValues(uint8_t mul, uint8_t add);
		Tex2D& TransformValues(float mul, float add);
		Tex2D& Flip();
		Tex2D& Upload(GLint internal_format = -1, GLint border = 0, GLint level = 0);
		Tex2D& UploadSRGBIfPossible(GLint internal_format = -1, GLint border = 0, GLint level = 0);
		Tex2D& CreateStorage(glm::tvec2<GLsizei> size, GLint internal_format, GLint levels = -1);
		Tex2D& CreateView(glm::tvec2<GLsizei> size, const TexInfo& origtexture, GLint internal_format, GLint minlevel = 0, GLint numlevels = 1);
		Tex2D& DestroyOffline();
		Tex2D& DestroyOnline();
		Tex2D& Destroy();

		Tex2DInfo GetInfo();

	private:
		GLsizei m_width, m_height, m_color_channels;
	};
}
