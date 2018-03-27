#include "DebugTexDisplayer.h"
#include "Model.h"
#include "Shader.h"

namespace e186
{

	DebugTexDisplayer::DebugTexDisplayer() :
		m_tweak_bar(Engine::current()->tweak_bar_manager().create_new_tweak_bar("Debug Textures"))
	{
		m_texture_configs.reserve(kVecSize);

		m_quad = Model::LoadFromFile("assets/models/quad01.obj", glm::mat4(1.0f), MOLF_default);
		assert(m_quad);
		m_quad->CreateAndUploadGpuData();
		m_quad->GenerateVAOsWithVertexAttribConfig(VertexAttribData::Position | VertexAttribData::Tex2D);

		m_shader.AddVertexShaderSourceFromFile("assets/shaders/texture.vert")
				.AddFragmentShaderSourceFromFile("assets/shaders/texture.frag", { std::make_tuple(0, "fragColor") })
				.Build();

		m_sampler_loc = m_shader.GetUniformLocation("sampler");
		m_transform_mul_loc = m_shader.GetUniformLocation("transform_mul");
		m_transform_add_loc = m_shader.GetUniformLocation("transform_add");
		m_pvmt_matrix_loc = m_shader.GetUniformLocation("pvmtMatrix");

		TwDefine(" 'Debug Textures' color='26 27 61' text=light position='232 400' ");
	}

	DebugTexDisplayer::~DebugTexDisplayer()
	{
	}

	DebugTexDisplayer::DbgTexConfig* DebugTexDisplayer::Add(GLsizei width, GLsizei height, GLenum texTarget, GLuint glHandle, std::string name)
	{
		auto n = m_texture_configs.size();
		if (n >= kVecSize)
		{
			log_error("Max. number of debug textures reached");
			return nullptr;
		}

		m_texture_configs.emplace_back(DebugTexDisplayer::DbgTexConfig{
			true,	   // bool m_enabled;
			false,	   // bool m_border;
			width,	   // GLsizei m_width;
			height,	   // GLsizei m_height;
			.5f,	   // float m_scale;
			true,	   // bool m_scale_relative_to_window_height;
			0,		   // GLint m_offset_x;
			0,		   // GLint m_offset_y;
			texTarget, // GLenum m_texture_target;
			glHandle,  // GLuint m_gl_handle;
			glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), // float m_transform_mul;
			glm::vec4(0.0f, 0.0f, 0.0f, 0.0f)  // float m_transform_add;
		});

		auto& element = m_texture_configs.back();
		auto num = "Tex #" + std::to_string(n);
		auto groupAssignment = " group='" + name + "' ";
		TwAddVarRW(m_tweak_bar, (num + " enbld").c_str(), TW_TYPE_BOOLCPP, &element.m_enabled, groupAssignment.c_str());
		TwAddVarRW(m_tweak_bar, (num + " off-x").c_str(), TW_TYPE_INT32, &element.m_offset_x, groupAssignment.c_str());
		TwAddVarRW(m_tweak_bar, (num + " off-y").c_str(), TW_TYPE_INT32, &element.m_offset_y, groupAssignment.c_str());
		TwAddVarRW(m_tweak_bar, (num + " scale").c_str(), TW_TYPE_FLOAT, &element.m_scale, (" min=0.01 max=2.0 step=0.01 " + groupAssignment).c_str());
		TwAddVarRW(m_tweak_bar, (num + " relsc").c_str(), TW_TYPE_BOOLCPP, &element.m_scale_relative_to_window_height, groupAssignment.c_str());
		TwAddVarRW(m_tweak_bar, (num + " t:mult").c_str(), TW_TYPE_DIR3F, glm::value_ptr(element.m_transform_mul), groupAssignment.c_str());
		TwAddVarRW(m_tweak_bar, (num + " t:add").c_str(), TW_TYPE_DIR3F, glm::value_ptr(element.m_transform_add), groupAssignment.c_str());
		return &element;
	}

	DebugTexDisplayer::DbgTexConfig* DebugTexDisplayer::Add(const TexInfo& data, std::string name)
	{	
		return Add(256, 256, data.target(), data.handle(), std::move(name));
	}

	DebugTexDisplayer::DbgTexConfig* DebugTexDisplayer::Add(const Tex2D& data, std::string name)
	{
		return Add(data.width(), data.height(), data.target(), data.handle(), std::move(name));
	}

	DebugTexDisplayer::DbgTexConfig* DebugTexDisplayer::Add(const FrameBufferObject& data, GLenum attachment, std::string name)
	{
		auto* texInfo = data.FindAttachedTexture(attachment);
		if (nullptr == texInfo)
		{
			log_error("Couldn't find attached texture at attachment %u, with name[%s]", attachment, name.c_str());
			return nullptr;
		}
		return Add(data.width(), data.height(), texInfo->target(), texInfo->handle(), std::move(name));
	}

	void DebugTexDisplayer::Render()
	{
		const auto wnd_width = Engine::current()->window_width();
		const auto wnd_height = Engine::current()->window_height();
		
		Camera orthoCam; 
		orthoCam.SetOrthogonalProjection(0.0f, static_cast<float>(wnd_width), 0.0f, static_cast<float>(wnd_height), 0.0f, 100.0f);
		auto pM = orthoCam.projection_matrix();

		glDisable(GL_DEPTH_TEST);
		glDepthMask(GL_FALSE);

		m_shader.Use();
		glActiveTexture(GL_TEXTURE0);
		glUniform1i(m_sampler_loc, 0); // 0 => GL_TEXTURE0

		auto& quad_mesh = m_quad->mesh_at(0);

		for (auto& tc : m_texture_configs)
		{
			if (!tc.m_enabled)
				continue;

			glBindTexture(tc.m_texture_target, tc.m_gl_handle);

			m_shader.SetUniform(m_transform_mul_loc, tc.m_transform_mul);
			m_shader.SetUniform(m_transform_add_loc, tc.m_transform_add);

			float sc_w, sc_h;
			if (tc.m_scale_relative_to_window_height)
			{
				sc_h = tc.m_scale * wnd_height;
				sc_w = sc_h * tc.m_width / tc.m_height;
			}
			else
			{
				sc_h = tc.m_height * tc.m_scale;
				sc_w = tc.m_width * tc.m_scale;
			}

			if (tc.m_border)
			{
				// make it black
				m_shader.SetUniform(m_transform_mul_loc, glm::vec4(0.f, 0.f, 0.f, 0.f));
				m_shader.SetUniform(m_transform_add_loc, glm::vec4(0.f, 0.f, 0.f, 0.f));

				glm::mat4 screenMb = pM
					* glm::translate(glm::vec3(tc.m_offset_x - 1.0f, tc.m_offset_y - 1.0f, 0.0f))
					* glm::scale(glm::vec3(sc_w + 2.0f, sc_h + 2.0f, 0.0f));
				m_shader.SetUniform(m_pvmt_matrix_loc, screenMb);
				RenderMesh(m_shader, quad_mesh);

				// change back to actual colors
				m_shader.SetUniform(m_transform_mul_loc, tc.m_transform_mul);
				m_shader.SetUniform(m_transform_add_loc, tc.m_transform_add);
			}

			glm::mat4 screenM = pM
				* glm::translate(glm::vec3(tc.m_offset_x, tc.m_offset_y, 0.0f))
				* glm::scale(glm::vec3(sc_w, sc_h, 0.0f));
			m_shader.SetUniform(m_pvmt_matrix_loc, screenM);
			RenderMesh(m_shader, quad_mesh);
		}

		glDepthMask(GL_TRUE);
		glEnable(GL_DEPTH_TEST);
	}
}
