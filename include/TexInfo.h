#pragma once
#include "ShaderType.h"

namespace e186
{
	enum struct TexParams : uint32_t
	{
		None					= 0x00000000,
		GenerateMipMaps			= 0x00000001,
		NearestFiltering		= 0x00000002,
		LinearFiltering			= 0x00000004,
		BilinearFiltering		= 0x00000008,
		TrilinearFiltering		= 0x00000010,
		AnisotropicFiltering2x	= 0x00000020,
		AnisotropicFiltering4x	= 0x00000040,
		AnisotropicFiltering8x	= 0x00000080,
		AnisotropicFiltering16x	= 0x00000100,
		AnisotropicFilteringMax = 0x00000200,
		ClampToEdge				= 0x00001000,
		ClampToBorder			= 0x00002000,
		Repeat					= 0x00004000,
		MirroredRepeat			= 0x00008000,
		EnableTextureCompareMode= 0x00010000,
	};

	inline TexParams operator| (TexParams a, TexParams b)
	{
		typedef std::underlying_type<TexParams>::type EnumType;
		return static_cast<TexParams>(static_cast<EnumType>(a) | static_cast<EnumType>(b));
	}

	inline TexParams operator& (TexParams a, TexParams b)
	{
		typedef std::underlying_type<TexParams>::type EnumType;
		return static_cast<TexParams>(static_cast<EnumType>(a) & static_cast<EnumType>(b));
	}

	inline TexParams& operator |= (TexParams& a, TexParams b)
	{
		return a = a | b;
	}

	inline TexParams& operator &= (TexParams& a, TexParams b)
	{
		return a = a & b;
	}

	inline TexParams operator ~ (const TexParams a)
	{
		typedef std::underlying_type<TexParams>::type EnumType;
		return static_cast<TexParams>(~static_cast<EnumType>(a));
	}


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
		TexInfo& BindAndSetTextureParameters(TexParams parameters);

		/// Ensure that the texture is bound before calling this method!
		TexInfo& GenerateMipMaps();
		const TexInfo& GenerateMipMaps() const;

		void DestroyOnline(bool is_destructing = false);
	};

	bool IsFormatSRGB(GLint format);
}
