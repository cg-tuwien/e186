#pragma once

namespace e186
{
	class FboAttachmentConfig
	{
		GLint m_internal_format;
		GLint m_format;
		GLenum m_data_type;
		GLint m_border;
	public:
		FboAttachmentConfig(GLint internalFormat, GLint imageFormat, GLenum type, GLint border = 0);
		GLint internal_format() const;
		GLint format() const;
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
		static FboAttachmentConfig kPresetSRGB;
		static FboAttachmentConfig kPresetSRGBA;
		static FboAttachmentConfig kPresetDepthStencil24_8;
		static FboAttachmentConfig kPresetDepth32F;
		static FboAttachmentConfig kPresetDepth24;
		static FboAttachmentConfig kPresetDepth16;
	};

	class FrameBufferObject
	{
		GLint m_viewport_x, m_viewport_y;
		GLsizei m_width, m_height;
		GLuint m_fbo_id;

		std::vector<GLenum> m_color_attachments;
		std::unordered_map<GLenum, std::shared_ptr<TexInfo>> m_tex_attachments;
			
		GLenum m_depth_buffer_format;
		GLuint m_depth_buffer_handle;
			
		glm::vec4 m_clear_color;

		void GenerateFbo();
		void AttachTexture(const FboAttachmentConfig& config, std::vector<GLenum> attachments, TexParams params);

	public:
		FrameBufferObject(GLsizei width, GLsizei height);
		FrameBufferObject(const FrameBufferObject& other) = delete;
		FrameBufferObject& operator=(const FrameBufferObject& other) = delete;
		FrameBufferObject(FrameBufferObject&& other) noexcept;
		FrameBufferObject& operator=(FrameBufferObject&& other) noexcept;
		~FrameBufferObject();

		static GLenum target();

		GLint viewport_x() const { return m_viewport_x; }
		GLint viewport_y() const { return m_viewport_y; }
		GLsizei width() const { return m_width; }
		GLsizei height() const { return m_height; }
		GLuint handle() const { return m_fbo_id; }
		void set_viewport_x(GLint value) { m_viewport_x = value; }
		void set_viewport_y(GLint value) { m_viewport_y = value; }
		void set_width(GLsizei value) { m_width = value; }
		void set_height(GLsizei value) { m_height = value; }

		const TexInfo* FindAttachedTexture(GLenum attachment) const;

		FrameBufferObject& Bind();
		FrameBufferObject& Unbind();
		FrameBufferObject& Clear(GLbitfield clearFlags = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
		FrameBufferObject& ClearAllColorBuffers();
		FrameBufferObject& ClearColor();
		FrameBufferObject& ClearDepth();
		FrameBufferObject& SetViewport();
		FrameBufferObject& set_clear_color(glm::vec4 color);

		FrameBufferObject& AttachComponent(
			FboAttachmentConfig config,
			GLenum attachment,
			TexParams params = TexParams::GenerateMipMaps | TexParams::TrilinearFiltering | TexParams::ClampToEdge);

		FrameBufferObject& AttachComponent(
			FboAttachmentConfig config,
			std::vector<GLenum> attachments,
			TexParams params = TexParams::GenerateMipMaps | TexParams::TrilinearFiltering | TexParams::ClampToEdge);

		FrameBufferObject& AttachRenderBuffer(FboAttachmentConfig config);

		const std::vector<GLenum>& GetAllColorBuffers() const;
		void DrawIntoColorBuffersInAttachOrder() const;

		/*!	Checks the fbo's status and prints an info-message. 
			*	If the fbo's status is GL_FRAMEBUFFER_COMPLETE, this method returns true; false otherwise.
			*	Make sure that the fbo is bound before calling this method.
			*/
		static bool status();

		void Destroy();
	};
}
