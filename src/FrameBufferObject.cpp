#include "FrameBufferObject.h"

namespace e186
{
#pragma region RenderTextureColorConfig

	FboAttachmentConfig::FboAttachmentConfig(GLint internalFormat, GLint imageFormat, GLenum type, GLint border, uint32_t num_samples) :
		m_internal_format(internalFormat),
		m_format(imageFormat),
		m_data_type(type),
		m_border(border),
		m_num_samples(num_samples)
	{
	}

	GLint FboAttachmentConfig::internal_format() const { return m_internal_format; }
	GLint FboAttachmentConfig::format() const { return m_format; }
	GLenum FboAttachmentConfig::data_type() const { return m_data_type; }
	GLint FboAttachmentConfig::border() const { return m_border; }
	uint32_t FboAttachmentConfig::num_samples() const { return m_num_samples; }

	void FboAttachmentConfig::set_internal_format(GLint value) { m_internal_format = value; }
	void FboAttachmentConfig::set_format(GLint value) { m_format = value; }
	void FboAttachmentConfig::set_data_type(GLenum value) { m_data_type = value; }
	void FboAttachmentConfig::set_border(GLint value) { m_border = value; }
	void FboAttachmentConfig::set_num_samples(uint32_t value) { m_num_samples = value; }
	FboAttachmentConfig& FboAttachmentConfig::modify_internal_format(GLint value) { set_internal_format(value); return *this; }
	FboAttachmentConfig& FboAttachmentConfig::modify_format(GLint value) { set_format(value); return *this; }
	FboAttachmentConfig& FboAttachmentConfig::modify_data_type(GLenum value) { set_data_type(value); return *this; }
	FboAttachmentConfig& FboAttachmentConfig::modify_border(GLint value) { set_border(value); return *this; }
	FboAttachmentConfig& FboAttachmentConfig::modify_num_samples(uint32_t value) { set_num_samples(value); return *this; }

	FboAttachmentConfig FboAttachmentConfig::kPresetNone			(0, 0, 0);
	FboAttachmentConfig FboAttachmentConfig::kPresetRGB				(GL_RGB,				GL_RGB,				GL_UNSIGNED_BYTE);
	FboAttachmentConfig FboAttachmentConfig::kPresetRGBA			(GL_RGBA,				GL_RGBA,			GL_UNSIGNED_BYTE);
	FboAttachmentConfig FboAttachmentConfig::kPresetRGBA8			(GL_RGBA8,				GL_RGBA,			GL_UNSIGNED_BYTE);
	FboAttachmentConfig FboAttachmentConfig::kPresetRGB16F			(GL_RGB16F,				GL_RGB,				GL_HALF_FLOAT);
	FboAttachmentConfig FboAttachmentConfig::kPresetRGBA16F			(GL_RGBA16F,			GL_RGBA,			GL_HALF_FLOAT);
	FboAttachmentConfig FboAttachmentConfig::kPresetRGB32F			(GL_RGB32F,				GL_RGB,				GL_FLOAT);
	FboAttachmentConfig FboAttachmentConfig::kPresetRGBA32F			(GL_RGBA32F,			GL_RGBA,			GL_FLOAT);
	FboAttachmentConfig FboAttachmentConfig::kPresetRGBAhalf		(GL_RGBA,				GL_RGBA,			GL_HALF_FLOAT);
	FboAttachmentConfig FboAttachmentConfig::kPresetSRGB			(GL_SRGB,				GL_RGB,				GL_UNSIGNED_BYTE);
	FboAttachmentConfig FboAttachmentConfig::kPresetSRGBA			(GL_SRGB_ALPHA,			GL_RGBA,			GL_UNSIGNED_BYTE);
	FboAttachmentConfig FboAttachmentConfig::kPresetDepthStencil24_8(GL_DEPTH_STENCIL,		GL_DEPTH_STENCIL,	GL_UNSIGNED_INT_24_8);
	FboAttachmentConfig FboAttachmentConfig::kPresetDepth32F		(GL_DEPTH_COMPONENT32F, GL_DEPTH_COMPONENT, GL_FLOAT);
	FboAttachmentConfig FboAttachmentConfig::kPresetDepth24			(GL_DEPTH_COMPONENT24,	GL_DEPTH_COMPONENT, GL_UNSIGNED_INT);
	FboAttachmentConfig FboAttachmentConfig::kPresetDepth16			(GL_DEPTH_COMPONENT16,	GL_DEPTH_COMPONENT, GL_UNSIGNED_INT);
#pragma endregion

#pragma region RenderTexture

	FrameBufferObject::FrameBufferObject(GLsizei width, GLsizei height) :
		m_viewport_x(0),
		m_viewport_y(0),
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
		m_viewport_x(other.m_viewport_x),
		m_viewport_y(other.m_viewport_y),
		m_height(other.m_height),
		m_fbo_id(other.m_fbo_id),
		m_color_attachments(std::move(other.m_color_attachments)),
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
		m_viewport_x = other.m_viewport_x;
		m_viewport_y = other.m_viewport_y;
		m_width = other.m_width;
		m_height = other.m_height;
		m_fbo_id = other.m_fbo_id;
		m_color_attachments = std::move(other.m_color_attachments);
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

	const TexInfo* FrameBufferObject::FindAttachedTexture(GLenum attachment) const
	{
		auto result = m_tex_attachments.find(attachment);
		if (result != m_tex_attachments.end()) 
		{
			return result->second.get();
		}
		else 
		{
			return nullptr;
		}
	}

	FrameBufferObject& FrameBufferObject::BindForReading()
	{
		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo_id);
		return *this;
	}

	FrameBufferObject& FrameBufferObject::BindForWriting()
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo_id);
		return *this;
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

		// TODO: Do something with SRGB? Or don't?

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
	
	FrameBufferObject& FrameBufferObject::ClearAllColorBuffers()
	{
		DrawIntoColorBuffersInAttachOrder();
		GLint n = static_cast<GLint>(m_color_attachments.size());
		for (GLint i = 0; i < n; ++i)
		{
			glClearBufferfv(GL_COLOR, i, glm::value_ptr(m_clear_color));
		}
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
		glViewport(m_viewport_x, m_viewport_y, m_width, m_height);
		return *this;
	}

	FrameBufferObject& FrameBufferObject::set_clear_color(glm::vec4 color)
	{
		m_clear_color = std::move(color);
		return *this;
	}
		
	void FrameBufferObject::AttachTexture(const FboAttachmentConfig& config, std::vector<GLenum> attachments, TexParams params)
	{
		assert(attachments.size() > 0);
		Bind();

		GLuint texHandle;
		glGenTextures(1, &texHandle);
		assert(0 != texHandle);

		auto num_samples = config.num_samples();
		auto is_multisample = num_samples > 1;

		auto ti = !is_multisample
			? std::make_shared<TexInfo>(GL_TEXTURE_2D, texHandle, config.internal_format(), config.format(), config.data_type(), config.border())
			: std::make_shared<TexInfo>(GL_TEXTURE_2D_MULTISAMPLE, texHandle, config.internal_format(), config.format(), config.data_type(), config.border());
		check_gl_error("AttachTexture after creating TexInfo");

		if (!is_multisample)
		{
			ti->BindAndSetTextureParameters(params);

			glTexImage2D(
				GL_TEXTURE_2D, 
				0, 
				config.internal_format(), 
				m_width, m_height, 
				config.border(), 
				config.format(), 
				config.data_type(), 
				nullptr);

			if ((params & TexParams::GenerateMipMaps) != TexParams::None)
			{
				ti->GenerateMipMaps(); // Allocate memory for the mipmaps
			}
		}
		else // multisample texture
		{
			ti->Bind();
			glTexImage2DMultisample(
				GL_TEXTURE_2D_MULTISAMPLE, 
				static_cast<GLsizei>(num_samples), 
				config.internal_format(), 
				m_width, m_height, 
				false);
			check_gl_error("AttachTexture after glTexImage2DMultisample");
		}

		// attach texture to the FBO
		for (int i = 0; i < attachments.size(); ++i)
		{
			glFramebufferTexture2D(target(), attachments[i], ti->target(), ti->handle(), 0);
			check_gl_error(std::string("AttachTexture after glFramebufferTexture2D [" + std::to_string(i) + "]").c_str());
		}

		ti->Unbind();
		Unbind();
			
		// we've gotten this far... so, store the data
		for (int i = 0; i < attachments.size(); ++i)
		{
			auto att = attachments[i];
			m_tex_attachments.insert({ att, ti });
			if (att >= GL_COLOR_ATTACHMENT0 && att <= GL_COLOR_ATTACHMENT31)
			{
				m_color_attachments.push_back(att);
			}
		}
	}

	FrameBufferObject& FrameBufferObject::AttachComponent(
		FboAttachmentConfig config,
		GLenum attachment,
		TexParams params)
	{
		AttachTexture(config, { attachment }, params);
		return *this;
	}

	FrameBufferObject& FrameBufferObject::AttachComponent(
		FboAttachmentConfig config,
		std::vector<GLenum> attachments,
		TexParams params)
	{
		AttachTexture(config, std::move(attachments), params);
		return *this;
	}

	FrameBufferObject& FrameBufferObject::AttachRenderBuffer(FboAttachmentConfig config)
	{
		Bind();

		GLuint rbHandle;
		glGenRenderbuffers(1, &rbHandle);

		glBindRenderbuffer(GL_RENDERBUFFER, rbHandle);
		glRenderbufferStorage(GL_RENDERBUFFER, config.internal_format(), m_width, m_height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0); // unbind since we're done setting it's SetTextureParameters

		// attach depth component to the FBO
		if (config.format() == GL_DEPTH_COMPONENT || config.format() == GL_DEPTH_STENCIL)
		{
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbHandle);
		}
		// ...and maybe also the stencil component
		if (config.format() == GL_DEPTH_STENCIL)
		{
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbHandle);
		}
			
		Unbind();

		m_depth_buffer_format = GL_RENDERBUFFER;
		m_depth_buffer_handle = rbHandle;

		check_gl_error("AttachRenderBuffer after glFramebufferRenderbuffer and unbind");
		return *this;
	}

	const std::vector<GLenum>& FrameBufferObject::GetAllColorBuffers() const
	{
		return m_color_attachments;
	}

	void FrameBufferObject::DrawIntoColorBuffersInAttachOrder() const
	{
		glDrawBuffers(static_cast<GLsizei>(m_color_attachments.size()), &m_color_attachments[0]);
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
