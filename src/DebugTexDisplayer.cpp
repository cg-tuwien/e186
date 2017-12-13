#include "DebugTexDisplayer.h"
#include "Model.h"
#include "Shader.h"

namespace e186
{

	DebugTexDisplayer::DebugTexDisplayer() 
	{
		m_texture_configs.reserve(kVecSize);
		auto& twBarManager = Engine::current->tweak_bar_manager();
		m_tweak_bar = twBarManager.create_new_tweak_bar("Textures");
	}

	DebugTexDisplayer::~DebugTexDisplayer()
	{
		auto& twBarManager = Engine::current->tweak_bar_manager();
		twBarManager.destroy_tweak_bar(m_tweak_bar);
	}

	DebugTexDisplayer::DbgTexConfig* DebugTexDisplayer::Add(GLsizei width, GLsizei height, GLenum texTarget, GLuint glHandle, std::string name)
	{
		if (m_texture_configs.size() >= kVecSize)
		{
			log_error("Max. number of debug textures reached");
			return nullptr;
		}

		m_texture_configs.emplace_back(DebugTexDisplayer::DbgTexConfig{
			true,
			false,
			width,
			height,
			1.0f,
			0,
			0,
			texTarget,
			glHandle
		});
		auto& element = m_texture_configs.back();
		auto n = m_texture_configs.size();
		auto num = "Tex #" + std::to_string(n);
		auto groupAssignment = " group='" + name + "' ";
		TwAddVarRW(m_tweak_bar, (num + " E").c_str(), TW_TYPE_BOOL32, &element.m_enabled, groupAssignment.c_str());
		TwAddVarRW(m_tweak_bar, (num + " x").c_str(), TW_TYPE_INT32, &element.m_offset_x, groupAssignment.c_str());
		TwAddVarRW(m_tweak_bar, (num + " y").c_str(), TW_TYPE_INT32, &element.m_offset_y, groupAssignment.c_str());
		TwAddVarRW(m_tweak_bar, (num + " s").c_str(), TW_TYPE_FLOAT, &element.m_scale, (" min=0.01 max=2.0 step=0.01 " + groupAssignment).c_str());
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
		auto& texInfo = data.GetAttachedTexture(attachment);
		return Add(data.width(), data.height(), texInfo.target(), texInfo.handle(), std::move(name));
	}

	void DebugTexDisplayer::Render()
	{
		static auto quad = []()
		{
			auto quad = Model::LoadFromFile("assets/models/quad01.obj", glm::mat4(1.0f), MOLF_default);
			assert(quad);
			quad->GenerateVAOsWithVertexAttribConfig(VertexAttribData_Position | VertexAttribData_Tex2D);
			return quad;
		}();
		static Shader shdr = []()
		{
			Shader shdr;
			shdr.AddVertexShaderSourceFromFile("assets/shaders/texture.vert")
				.AddFragmentShaderSourceFromFile("assets/shaders/texture.frag", { std::make_tuple(0, "fragColor") })
				.Build();
			return shdr;
		}();
		static auto samplerLoc = shdr.GetUniformLocation("sampler");
		static auto colorLoc = shdr.GetUniformLocation("color");
		static auto pvmtMatrixLoc = shdr.GetUniformLocation("pvmtMatrix");

		auto width = Engine::current->window_width();
		auto height = Engine::current->window_height();
		
		Camera orthoCam; orthoCam.SetOrthogonalProjection(0.0f, width, 0.0f, height, 0.0f, 100.0f);
		auto pM = orthoCam.projection_matrix();

		glDisable(GL_DEPTH_TEST);
		glDepthMask(GL_FALSE);

		glUseProgram(shdr.handle());
		glActiveTexture(GL_TEXTURE0);
		glUniform1i(samplerLoc, 0); // 0 => GL_TEXTURE0
		glUniform4f(colorLoc, 1.0f, 1.0f, 1.0f, 1.0f);

		for (auto& tc : m_texture_configs)
		{
			if (!tc.m_enabled)
				continue;

			glBindTexture(tc.m_texture_target, tc.m_gl_handle);

			if (tc.m_border)
			{
				// make it black
				glUniform4f(colorLoc, 0.0f, 0.0f, 0.0f, 1.0f);

				glm::mat4 screenMb = pM
					* glm::translate(glm::vec3(tc.m_offset_x - 1.0f, tc.m_offset_y - 1.0f, 0.0f))
					* glm::scale(glm::vec3(tc.m_width * tc.m_scale + 2.0f, tc.m_height * tc.m_scale + 2.0f, 0.0f));
				glUniformMatrix4fv(pvmtMatrixLoc, 1, GL_FALSE, glm::value_ptr(screenMb));
				quad->RenderForVertexAttribConfig(VertexAttribData_Position | VertexAttribData_Tex2D, GL_TRIANGLES);

				// change back to actual colors
				glUniform4f(shdr.GetUniformLocation("color"), 1.0f, 1.0f, 1.0f, 1.0f);
			}

			glm::mat4 screenM = pM
				* glm::translate(glm::vec3(tc.m_offset_x, tc.m_offset_y, 0.0f))
				* glm::scale(glm::vec3(tc.m_width * tc.m_scale, tc.m_height * tc.m_scale, 0.0f));
			glUniformMatrix4fv(pvmtMatrixLoc, 1, GL_FALSE, glm::value_ptr(screenM));
			quad->RenderForVertexAttribConfig(VertexAttribData_Position | VertexAttribData_Tex2D, GL_TRIANGLES);
		}

		glDepthMask(GL_TRUE);
		glEnable(GL_DEPTH_TEST);
	}
}
