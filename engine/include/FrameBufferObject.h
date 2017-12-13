#pragma once

namespace e186
{
	class FboRenderBufferConfig
	{
		GLenum m_internal_format;
		GLenum m_depth_attachment;
		GLenum m_stencil_attachment;
	public:
		FboRenderBufferConfig(GLenum internalFormat, GLenum depthAttachment, GLenum stencilAttachment);
		GLint internal_format() const;
		GLenum depth_attachment() const;
		GLenum stencil_attachment() const;

		static FboRenderBufferConfig kPresetNone;
		static FboRenderBufferConfig kPresetDepthStencil;
		static FboRenderBufferConfig kPresetDepth;
		static FboRenderBufferConfig kPresetDepth16;
	};

	class FboAttachmentConfig
	{
		GLint m_internal_format;
		GLint m_image_format;
		GLenum m_data_type;
		GLint m_border;
	public:
		FboAttachmentConfig(GLint internalFormat, GLint imageFormat, GLenum type, GLint border = 0);
		GLint internal_format() const;
		GLint image_format() const;
		GLenum data_type() const;
		GLint border() const;

		static FboAttachmentConfig kPresetNone;
		static FboAttachmentConfig kPresetRGB;
		static FboAttachmentConfig kPresetRGBA;
		static FboAttachmentConfig kPresetRGB16F;
		static FboAttachmentConfig kPresetRGBA16F;
		static FboAttachmentConfig kPresetRGB32F;
		static FboAttachmentConfig kPresetRGBA32F;
		static FboAttachmentConfig kPresetRGBAhalf;
	};

	class FrameBufferObject
	{
		GLsizei m_width, m_height;
		GLuint m_fbo_id;

		std::unordered_map<GLenum, TexInfo> m_tex_attachments;
			
		GLenum m_depth_buffer_format;
		GLuint m_depth_buffer_handle;
			
		glm::vec4 m_clear_color;

		void GenerateFbo();
		void AttachTexture(const FboAttachmentConfig& config, GLenum attachment, unsigned int params);

	public:
		FrameBufferObject(GLsizei width, GLsizei height);
		FrameBufferObject(const FrameBufferObject& other) = delete;
		FrameBufferObject& operator=(const FrameBufferObject& other) = delete;
		FrameBufferObject(FrameBufferObject&& other) noexcept;
		FrameBufferObject& operator=(FrameBufferObject&& other) noexcept;
		~FrameBufferObject();

		static GLenum target();

		GLsizei width() const {
			return m_width;
		}

		GLsizei height() const {
			return m_height;
		}

		GLuint handle() const {
			return m_fbo_id;
		}

		const TexInfo& GetAttachedTexture(GLenum attachment) const;
		const TexInfo* FindAttachedTexture(GLenum attachment) const;

		FrameBufferObject& Bind();
		FrameBufferObject& Unbind();
		FrameBufferObject& Clear(GLbitfield clearFlags = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
		FrameBufferObject& ClearColor();
		FrameBufferObject& ClearDepth();
		FrameBufferObject& SetViewport();
		FrameBufferObject& set_clear_color(glm::vec4 color);

		FrameBufferObject& AttachColorComponent(
			FboAttachmentConfig config, 
			GLenum attachment = GL_COLOR_ATTACHMENT0, 
			unsigned int params = TexParams_GenerateMipMaps | TexParams_TrilinearFiltering | TexParams_ClampToEdge);

		FrameBufferObject& AttachDepthComponent(
			FboAttachmentConfig config,
			GLenum attachment = GL_DEPTH_ATTACHMENT,
			unsigned int params = TexParams_GenerateMipMaps | TexParams_TrilinearFiltering | TexParams_ClampToEdge);

		FrameBufferObject& AttachRenderBuffer(FboRenderBufferConfig config);

		/*!	Checks the fbo's status and prints an info-message. 
			*	If the fbo's status is GL_FRAMEBUFFER_COMPLETE, this method returns true; false otherwise.
			*	Make sure that the fbo is bound before calling this method.
			*/
		static bool status();

		void Destroy();
	};
}
