#include "FrameBufferObject.h"

namespace e186
{
#pragma region RenderTextureRenderBufferConfig

	FboRenderBufferConfig::FboRenderBufferConfig(GLenum internalFormat, GLenum depthAttachment, GLenum stencilAttachment) :
		m_internal_format(internalFormat),
		m_depth_attachment(depthAttachment),
		m_stencil_attachment(stencilAttachment)
	{
	}

	GLint FboRenderBufferConfig::internal_format() const
	{
		return m_internal_format;
	}

	GLenum FboRenderBufferConfig::depth_attachment() const
	{
		return m_depth_attachment;
	}

	GLenum FboRenderBufferConfig::stencil_attachment() const
	{
		return m_stencil_attachment;
	}

	FboRenderBufferConfig FboRenderBufferConfig::kPresetNone			(0,							0,									0);
	FboRenderBufferConfig FboRenderBufferConfig::kPresetDepthStencil(GL_DEPTH_STENCIL,			GL_DEPTH_STENCIL_ATTACHMENT,		0);
	FboRenderBufferConfig FboRenderBufferConfig::kPresetDepth16		(GL_DEPTH_COMPONENT16,		GL_DEPTH_ATTACHMENT,				0);

#pragma endregion


#pragma region RenderTextureColorConfig

	FboAttachmentConfig::FboAttachmentConfig(GLint internalFormat, GLint imageFormat, GLenum type, GLint border) :
		m_internal_format(internalFormat),
		m_image_format(imageFormat),
		m_data_type(type),
		m_border(border)
	{
	}

	GLint FboAttachmentConfig::internal_format() const
	{
		return m_internal_format;
	}

	GLint FboAttachmentConfig::image_format() const
	{
		return m_image_format;
	}

	GLenum FboAttachmentConfig::data_type() const
	{
		return m_data_type;
	}

	GLint FboAttachmentConfig::border() const
	{
		return m_border;
	}

	FboAttachmentConfig FboAttachmentConfig::kPresetNone	(0, 0, 0);
	FboAttachmentConfig FboAttachmentConfig::kPresetRGB		(GL_RGB,		GL_RGB,			GL_UNSIGNED_BYTE);
	FboAttachmentConfig FboAttachmentConfig::kPresetRGBA	(GL_RGBA,		GL_RGBA,		GL_UNSIGNED_BYTE);
	FboAttachmentConfig FboAttachmentConfig::kPresetRGB16F	(GL_RGB16F,		GL_RGB,			GL_HALF_FLOAT);
	FboAttachmentConfig FboAttachmentConfig::kPresetRGBA16F	(GL_RGBA16F,	GL_RGBA,		GL_HALF_FLOAT);
	FboAttachmentConfig FboAttachmentConfig::kPresetRGB32F	(GL_RGB32F,		GL_RGB,			GL_FLOAT);
	FboAttachmentConfig FboAttachmentConfig::kPresetRGBA32F	(GL_RGBA32F,	GL_RGBA,		GL_FLOAT);
	FboAttachmentConfig FboAttachmentConfig::kPresetRGBAhalf(GL_RGBA,		GL_RGBA,		GL_HALF_FLOAT);

#pragma endregion

#pragma region RenderTexture

	FrameBufferObject::FrameBufferObject(GLsizei width, GLsizei height) :
		m_width(width),
		m_height(height),
		m_fbo_id(0),
		m_depth_buffer_format(0),
		m_depth_buffer_handle(0),
		m_clear_color(.153f, .275f, .349f, 1.0f)
	{
		GenerateFbo();
	}

	FrameBufferObject::FrameBufferObject(FrameBufferObject&& other) noexcept :
		m_width(other.m_width),
		m_height(other.m_height),
		m_fbo_id(other.m_fbo_id),
		m_tex_attachments(std::move(other.m_tex_attachments)),
		m_depth_buffer_format(other.m_depth_buffer_format),
		m_depth_buffer_handle(other.m_depth_buffer_handle),
		m_clear_color(std::move(other.m_clear_color))
	{
		other.m_width = 0;
		other.m_height = 0;
		other.m_fbo_id = 0;
		other.m_depth_buffer_format = 0;
		other.m_depth_buffer_handle = 0;
		
		log_debug("Move constructing FrameBufferObject with m_fbo_id[%u]", m_fbo_id);
	}

	FrameBufferObject& FrameBufferObject::operator=(FrameBufferObject&& other) noexcept 
	{
		m_width = other.m_width;
		m_height = other.m_height;
		m_fbo_id = other.m_fbo_id;
		m_tex_attachments = std::move(other.m_tex_attachments);
		m_depth_buffer_format = other.m_depth_buffer_format;
		m_depth_buffer_handle = other.m_depth_buffer_handle;
		m_clear_color = std::move(other.m_clear_color);

		other.m_width = 0;
		other.m_height = 0;
		other.m_fbo_id = 0;
		other.m_depth_buffer_format = 0;
		other.m_depth_buffer_handle = 0;

		log_debug("Move assigning FrameBufferObject with m_fbo_id[%u]", m_fbo_id);
		return *this;
	}

	FrameBufferObject::~FrameBufferObject()
	{
		if (0 != m_fbo_id)
		{
			log_debug("Destructing FrameBufferObject with m_fbo_id[%u]", m_fbo_id);
		}
		Destroy();
	}

	void FrameBufferObject::GenerateFbo()
	{
		glGenFramebuffers(1, &m_fbo_id);
		assert(0 != m_fbo_id);
	}

	GLenum FrameBufferObject::target()
	{
		return GL_FRAMEBUFFER;
	}

	const TexInfo& FrameBufferObject::GetAttachedTexture(GLenum attachment) const
	{
		return m_tex_attachments.at(attachment);
	}

	const TexInfo* FrameBufferObject::FindAttachedTexture(GLenum attachment) const
	{
		auto result = m_tex_attachments.find(attachment);
		if (result != m_tex_attachments.end()) 
		{
			return &result->second;
		}
		else 
		{
			return nullptr;
		}
	}

	FrameBufferObject& FrameBufferObject::Bind()
	{
		glBindFramebuffer(target(), m_fbo_id);

		if (IsFormatSRGB(0)) // TODO: which one??
		{
			glEnable(GL_FRAMEBUFFER_SRGB); // TODO when to??
		}

		return *this;
	}

	FrameBufferObject& FrameBufferObject::Unbind()
	{
		glBindFramebuffer(target(), 0);

		// Enabling SRGB is the default, since the stuff rendered to the screen should be SRGB
		glEnable(GL_FRAMEBUFFER_SRGB);

		return *this;
	}

	FrameBufferObject& FrameBufferObject::Clear(GLbitfield clearFlags)
	{
		if ((GL_COLOR_BUFFER_BIT & clearFlags) != 0)
		{
			glClearColor(m_clear_color.r, m_clear_color.g, m_clear_color.b, m_clear_color.a);
		}
		glClear(clearFlags);
		return *this;
	}

	FrameBufferObject& FrameBufferObject::ClearColor()
	{
		glClearColor(m_clear_color.r, m_clear_color.g, m_clear_color.b, m_clear_color.a);
		glClear(GL_COLOR_BUFFER_BIT);
		return *this;
	}

	FrameBufferObject& FrameBufferObject::ClearDepth()
	{
		glClear(GL_DEPTH_BUFFER_BIT);
		return *this;
	}

	FrameBufferObject& FrameBufferObject::SetViewport()
	{
		glViewport(0, 0, m_width, m_height);
		return *this;
	}

	FrameBufferObject& FrameBufferObject::set_clear_color(glm::vec4 color)
	{
		m_clear_color = std::move(color);
		return *this;
	}
		
	void FrameBufferObject::AttachTexture(const FboAttachmentConfig& config, GLenum attachment, unsigned int params)
	{
		Bind();

		GLuint texHandle;
		glGenTextures(1, &texHandle);
		assert(0 != texHandle);

		TexInfo ti(GL_TEXTURE_2D, texHandle, config.internal_format(), config.image_format(), config.data_type(), config.border());
		ti.SetTextureParameters(params);

		glTexImage2D(GL_TEXTURE_2D, 0, config.internal_format(), m_width, m_height, config.border(), config.image_format(), config.data_type(), nullptr);
		ti.GenerateMipMaps(); // /!\ Allocate the mipmaps /!\

		// attach texture to the FBO
		glFramebufferTexture2D(target(), attachment, ti.target(), ti.handle(), 0);

		check_gl_error("AttachTexture after glFramebufferTexture2D and unbind");

		ti.Unbind();
		Unbind();
			
		// we've gotten this far... so, store the data
		m_tex_attachments.insert({ attachment, std::move(ti) });
	}

	FrameBufferObject& FrameBufferObject::AttachColorComponent(FboAttachmentConfig config, GLenum attachment, unsigned int params)
	{
		AttachTexture(config, attachment, params);
		return *this;
	}

	FrameBufferObject& FrameBufferObject::AttachDepthComponent(FboAttachmentConfig config, GLenum attachment, unsigned int params)
	{
		AttachTexture(config, attachment, params);
		return *this;
	}

	FrameBufferObject& FrameBufferObject::AttachRenderBuffer(FboRenderBufferConfig config)
	{
		Bind();

		GLuint rbHandle;
		glGenRenderbuffers(1, &rbHandle);

		glBindRenderbuffer(GL_RENDERBUFFER, rbHandle);
		glRenderbufferStorage(GL_RENDERBUFFER, config.internal_format(), m_width, m_height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0); // unbind since we're done setting it's SetTextureParameters

		// attach depth component to the FBO
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, config.depth_attachment(), GL_RENDERBUFFER, rbHandle);
		// ...and maybe also the stencil component
		if (config.stencil_attachment() > 0)
		{
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, config.stencil_attachment(), GL_RENDERBUFFER, rbHandle);
		}
			
		Unbind();

		m_depth_buffer_format = GL_RENDERBUFFER;
		m_depth_buffer_handle = rbHandle;

		check_gl_error("AttachRenderBuffer after glFramebufferRenderbuffer and unbind");
		return *this;
	}

	bool FrameBufferObject::status()
	{
		auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		bool isGood = GL_FRAMEBUFFER_COMPLETE == status;
		log_info("FrameBuffer's status is [%u], i.e. status[%s]", status, isGood ? "true" : "false");
		return isGood;
	}

	void FrameBufferObject::Destroy()
	{
		if (0 == m_fbo_id || 0 == m_depth_buffer_format)
		{
			log_warning("Cannot destroy FrameBufferObject because of m_fbo_id[%d] or m_depth_buffer_format[%d]", m_fbo_id, m_depth_buffer_format);
			return;
		}

		if (GL_RENDERBUFFER == m_depth_buffer_format)
		{
			glDeleteRenderbuffers(1, &m_depth_buffer_handle);
		}
		else
		{
			// TOOD: Destroy depth texture
		}

		// TODO: Destroy color textures
		glDeleteFramebuffers(1, &m_fbo_id);

		m_depth_buffer_handle = 0;
		m_fbo_id = 0;
	}

#pragma endregion
}
