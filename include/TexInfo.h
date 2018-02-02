#pragma once

namespace e186
{
	enum TexParams : unsigned int
	{
		TexParams_GenerateMipMaps			= 0x00000001,
		TexParams_NearestFiltering			= 0x00000002,
		TexParams_LinearFiltering			= 0x00000004,
		TexParams_BilinearFiltering			= 0x00000008,
		TexParams_TrilinearFiltering		= 0x00000010,
		TexParams_AnisotropicFiltering2x	= 0x00000020,
		TexParams_AnisotropicFiltering4x	= 0x00000040,
		TexParams_AnisotropicFiltering8x	= 0x00000080,
		TexParams_AnisotropicFiltering16x	= 0x00000100,
		TexParams_AnisotropicFilteringMax   = 0x00000200,
		TexParams_ClampToEdge				= 0x00001000,
		TexParams_Repeat					= 0x00002000,
		TexParams_MirroredRepeat			= 0x00004000,
	};

	class TexInfo
	{
	protected:
		GLenum m_texture_target;
		GLuint m_gl_handle;

		GLint m_internal_format;
		GLint m_image_format;
		GLenum m_data_type;
		GLint m_border;

	public:
		static const GLint kUninitialized = -1;

		TexInfo(GLenum texTarget);
		TexInfo(GLenum texTarget, GLuint handle, GLint internalFormat, GLint imageFormat, GLenum type, GLint border);
		TexInfo(const TexInfo& other) = delete;
		TexInfo& operator=(const TexInfo& other) = delete;
		TexInfo(TexInfo&& other) noexcept;
		TexInfo& operator=(TexInfo&& other) noexcept;
		virtual ~TexInfo();

		explicit operator GLuint() const;
		GLenum target() const;
		GLuint handle() const;
		TexInfo& Bind();
		const TexInfo& Bind() const;
		void Unbind() const;

		GLint internal_format() const;
		GLint image_format() const;
		GLenum data_type() const;

		/// Will also bind the texture unconditionally
		TexInfo& SetTextureParameters(unsigned int parameters);

		/// Ensure that the texture is bound before calling this method!
		TexInfo& GenerateMipMaps();
		const TexInfo& GenerateMipMaps() const;

		void DestroyOnline(bool is_destructing = false);
	};

	bool IsFormatSRGB(GLint format);
}
