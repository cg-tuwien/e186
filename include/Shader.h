#pragma once
#include <unordered_set>
#include <set>
#include <array>

namespace e186
{
	struct MeshUniformSettersForShader;
	struct MeshRenderData;

	class Shader
	{
		static GLuint Compile(const std::string& complete_source, GLenum shaderType);
		void CheckErrorAndPrintInfoLog(const char* gl_error_location_hint, const char* info_log_description);
		void PrintInfoLog(const char* info_log_description);
		void DetermineVertexAttribConfig();
		void DeterminePrimitivesMode();

		static std::string LoadFromFile(const std::string& path);

		void DetermineWhichAutoMatsToCalc();
		void PrepareAutoMatActionConfigs();
		void CreateAutoMatCalcers();
		//static std::vector<const GLchar*> GetAsCStrs(const std::vector<std::string>& string_array);
		static std::string ConcatSources(const std::vector<std::string>& sources);

	public:
		Shader();
		Shader(const Shader& other) = delete;
		Shader(Shader&& other) noexcept;
		Shader& operator=(const Shader& other) = delete;
		Shader& operator=(Shader&& other) noexcept;
		~Shader();

		explicit operator GLuint() const;
		GLuint handle() const;

		static std::string version_string();

		Shader& AddVertexShaderSource(std::string shader_source, bool append_newline = true);
		Shader& AddTessellationControlShaderSource(std::string shader_source, bool append_newline = true);
		Shader& AddTessellationEvaluationShaderSource(std::string shader_source, bool append_newline = true);
		Shader& AddGeometryShaderSource(std::string shader_source, bool append_newline = true);
		Shader& AddFragmentShaderSource(std::string shader_source, bool append_newline = true);
		Shader& AddFragmentShaderSource(std::string shader_source, std::vector<std::tuple<GLuint, const GLchar*>> outputs, bool append_newline = true);
		Shader& AddFragmentShaderOutput(std::vector<std::tuple<GLuint, const GLchar*>> outputs);
		Shader& AddComputeShaderSource(std::string shader_source, bool append_newline = true);
		Shader& AddToMultipleShaderSources(std::string shader_source, ShaderType which_shaders, bool append_newline = true);
		Shader& AddVertexShaderSourceFromFile(std::string path, bool append_newline = true);
		Shader& AddTessellationControlShaderSourceFromFile(std::string path, bool append_newline = true);
		Shader& AddTessellationEvaluationShaderSourceFromFile(std::string path, bool append_newline = true);
		Shader& AddGeometryShaderSourceFromFile(std::string path, bool append_newline = true);
		Shader& AddFragmentShaderSourceFromFile(std::string path, std::vector<std::tuple<GLuint, const GLchar*>> outputs, bool append_newline = true);
		Shader& AddComputeShaderSourceFromFile(std::string path, bool append_newline = true);
		Shader& SetTransformFeedbackVaryings(std::vector<const char*> varyings, GLenum buffer_mode);
		Shader& Build();
		Shader& QueryOptionalUniformLocations(const std::vector<std::string>& names);
		Shader& QueryUniformLocations(const std::vector<std::string>& names);
		Shader& QueryMandatoryUniformLocations(const std::vector<std::string>& names);
		Shader& QueryOptionalUniformLocation(const std::string& name);
		Shader& QueryUniformLocation(const std::string& name);
		Shader& QueryMandatoryUniformLocation(const std::string& name);
		Shader& QueryOptionalUniformBlockIndex(const std::string& name);
		Shader& QueryUniformBlockIndex(const std::string& name);
		Shader& QueryMandatoryUniformBlockIndex(const std::string& name);
		Shader& DeclareAutoMatrix(std::string name, AutoMatrix properties);
		Shader& Destroy();
		bool HasUniform(const std::string& name) const;
		GLuint GetOptionalUniformLocation(const std::string& name);
		GLuint GetUniformLocation(const std::string& name);
		GLuint GetMandatoryUniformLocation(const std::string& name);
		GLuint GetOptionalUniformBlockIndex(const std::string& name);
		GLuint GetUniformBlockIndex(const std::string& name);
		GLuint GetMandatoryUniformBlockIndex(const std::string& name);
		bool has_tessellation_shaders() const;
		bool has_geometry_shader() const;
		VertexAttribData vertex_attrib_config() const;
		GLenum kind_of_primitives() const;
		void set_kind_of_primitives(GLenum mode);
		void SetAutoMatrices(const glm::mat4& transformationMatrix, const glm::mat4& modelMatrix, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix);
		GLint QueryPatchVertices();

		void Use() const 
		{
			glUseProgram(m_prog_handle);
		}

		void SetUniform(GLuint location, const glm::vec4& value) const
		{
			glUniform4fv(location, 1, static_cast<const GLfloat*>(glm::value_ptr(value)));
		}

		void SetUniform(GLuint location, GLfloat x, GLfloat y, GLfloat z, GLfloat w) const
		{
			glUniform4f(location, x, y, z, w);
		}

		void SetUniform(GLuint location, const glm::vec3& value) const
		{
			glUniform3fv(location, 1, static_cast<const GLfloat*>(glm::value_ptr(value)));
		}

		void SetUniform(GLuint location, GLfloat x, GLfloat y, GLfloat z) const
		{
			glUniform3f(location, x, y, z);
		}

		void SetUniform(GLuint location, GLfloat value) const
		{
			glUniform1f(location, value);
		}

		void SetUniform(GLuint location, GLint value) const
		{
			glUniform1i(location, value);
		}

		void SetUniform(GLuint location, const glm::vec2& value) const
		{
			glUniform2fv(location, 1, static_cast<const GLfloat*>(glm::value_ptr(value)));
		}

		void SetUniform(GLuint location, GLfloat x, GLfloat y) const
		{
			glUniform2f(location, x, y);
		}

		void SetUniform(GLuint location, const glm::mat3& value) const
		{
			glUniformMatrix3fv(location, 1, GL_FALSE, static_cast<const GLfloat*>(glm::value_ptr(value)));
		}

		void SetUniform(GLuint location, const glm::mat4& value) const
		{
			glUniformMatrix4fv(location, 1, GL_FALSE, static_cast<const GLfloat*>(glm::value_ptr(value)));
		}

		void SetUniformBlockBinding(GLuint location, GLuint block_binding) const
		{
			glUniformBlockBinding(m_prog_handle, location, block_binding);
		}

		void SetSampler(GLuint location, GLenum tex_target, GLuint tex_handle, GLuint texture_unit = 0) const
		{
			glActiveTexture(static_cast<GLenum>(GL_TEXTURE0 + texture_unit));
			glBindTexture(tex_target, tex_handle);
			glUniform1i(location, static_cast<GLint>(texture_unit));
		}

		void SetFirstSampler(GLuint location, GLenum tex_target, GLuint tex_handle, GLuint first_texture_unit = 0)
		{
			m_sampler_auto_index = first_texture_unit;
			SetSampler(location, tex_target, tex_handle, m_sampler_auto_index);
		}

		void SetNextSampler(GLuint location, GLenum tex_target, GLuint tex_handle, GLuint sampler_increment = 1)
		{
			m_sampler_auto_index += sampler_increment;
			SetSampler(location, tex_target, tex_handle, m_sampler_auto_index);
		}

		void SetSampler(GLuint location, const TexInfo& value, GLuint texture_unit = 0) const
		{
			glActiveTexture(static_cast<GLenum>(GL_TEXTURE0 + texture_unit));
			value.Bind();
			glUniform1i(location, static_cast<GLint>(texture_unit));
		}

		void SetFirstSampler(GLuint location, const TexInfo& value, GLuint first_texture_unit = 0)
		{	
			m_sampler_auto_index = first_texture_unit;
			SetSampler(location, value, m_sampler_auto_index);
		}

		void SetNextSampler(GLuint location, const TexInfo& value, GLuint sampler_increment = 1)
		{
			m_sampler_auto_index += sampler_increment;
			SetSampler(location, value, m_sampler_auto_index);
		}
			
		void SetImageTexture(GLuint location, const TexInfo& value, GLuint unit, GLint level, GLboolean layered, GLint layer, GLenum access)
		{
			glBindImageTexture(unit, value.handle(), level, layered, layer, access, value.internal_format());
			glUniform1i(location, static_cast<GLint>(unit));
		}

		void SetLight(GLuint color_loc, const AmbientLightGpuData& data)
		{
			SetUniform(color_loc, data.m_light_color);
		}

		void SetLight(GLuint position_loc, GLuint color_loc, GLuint attenuation_loc, const PointLightGpuData& data)
		{
			SetUniform(position_loc, data.m_position);
			SetUniform(color_loc, data.m_light_color);
			SetUniform(attenuation_loc, data.m_attenuation);
		}

		void SetLight(GLuint direction_loc, GLuint color_loc, const DirectionalLightGpuData& data)
		{
			SetUniform(direction_loc, data.m_light_dir_vs);
			SetUniform(color_loc, data.m_light_color);
		}


		template <typename... Args>
		void SetOptionalUniform(const std::string& uniform_name, Args&&... args)
		{
			const auto loc = GetOptionalUniformLocation(uniform_name);
			if (-1 != loc)
				SetUniform(loc, std::forward<Args>(args)...);
		}

		template <typename... Args>
		void SetUniform(const std::string& uniform_name, Args&&... args)
		{
			SetUniform(GetUniformLocation(uniform_name), std::forward<Args>(args)...);
		}

		template <typename... Args>
		void SetMandatoryUniform(const std::string& uniform_name, Args&&... args)
		{
			SetUniform(GetMandatoryUniformLocation(uniform_name), std::forward<Args>(args)...);
		}


		template <typename... Args>
		void SetOptionalUniformBlockBinding(const std::string& block_name, Args&&... args)
		{
			const auto loc = GetOptionalUniformBlockIndex(block_name);
			if (-1 != loc)
				SetUniformBlockBinding(loc, std::forward<Args>(args)...);
		}

		template <typename... Args>
		void SetUniformBlockBinding(const std::string& block_name, Args&&... args)
		{
			SetUniform(GetUniformBlockIndex(block_name), std::forward<Args>(args)...);
		}

		template <typename... Args>
		void SetMandatoryUniformBlockBinding(const std::string& block_name, Args&&... args)
		{
			SetUniform(GetMandatoryUniformBlockIndex(block_name), std::forward<Args>(args)...);
		}


		template <typename... Args>
		void SetOptionalSampler(const std::string& uniform_name, Args&&... args)
		{
			const auto loc = GetOptionalUniformLocation(uniform_name);
			if (-1 != loc)
				SetSampler(loc, std::forward<Args>(args)...);
		}

		template <typename... Args>
		void SetSampler(const std::string& uniform_name, Args&&... args)
		{
			SetSampler(GetUniformLocation(uniform_name), std::forward<Args>(args)...);
		}

		template <typename... Args>
		void SetMandatorySampler(const std::string& uniform_name, Args&&... args)
		{
			SetSampler(GetMandatoryUniformLocation(uniform_name), std::forward<Args>(args)...);
		}

		template <typename... Args>
		void SetOptionalFirstSampler(const std::string& uniform_name, Args&&... args)
		{
			const auto loc = GetOptionalUniformLocation(uniform_name);
			if (-1 != loc)
				SetFirstSampler(loc, std::forward<Args>(args)...);
		}

		template <typename... Args>
		void SetFirstSampler(const std::string& uniform_name, Args&&... args)
		{
			SetFirstSampler(GetUniformLocation(uniform_name), std::forward<Args>(args)...);
		}

		template <typename... Args>
		void SetMandatoryFirstSampler(const std::string& uniform_name, Args&&... args)
		{
			SetFirstSampler(GetMandatoryUniformLocation(uniform_name), std::forward<Args>(args)...);
		}

		template <typename... Args>
		void SetOptionalNextSampler(const std::string& uniform_name, Args&&... args)
		{
			const auto loc = GetOptionalUniformLocation(uniform_name);
			if (-1 != loc)
				SetNextSampler(loc, std::forward<Args>(args)...);
		}

		template <typename... Args>
		void SetNextSampler(const std::string& uniform_name, Args&&... args)
		{
			SetNextSampler(GetUniformLocation(uniform_name), std::forward<Args>(args)...);
		}

		template <typename... Args>
		void SetMandatoryNextSampler(const std::string& uniform_name, Args&&... args)
		{
			SetNextSampler(GetMandatoryUniformLocation(uniform_name), std::forward<Args>(args)...);
		}


		template <typename... Args>
		void SetOptionalImageTexture(const std::string& uniform_name, Args&&... args)
		{
			const auto loc = GetOptionalUniformLocation(uniform_name);
			if (-1 != loc)
				SetImageTexture(loc, std::forward<Args>(args)...);
		}

		template <typename... Args>
		void SetImageTexture(const std::string& uniform_name, Args&&... args)
		{
			SetImageTexture(GetUniformLocation(uniform_name), std::forward<Args>(args)...);
		}

		template <typename... Args>
		void SetMandatoryImageTexture(const std::string& uniform_name, Args&&... args)
		{
			SetImageTexture(GetMandatoryUniformLocation(uniform_name), std::forward<Args>(args)...);
		}


		void SetOptionalLight(const std::string& uniform_name, const AmbientLightGpuData& data, const char* color_member = ".color")
		{
			const auto color_loc = GetOptionalUniformLocation(uniform_name + color_member);
			if (-1 != color_loc) 
				SetLight(color_loc, data);
		}
		void SetLight(const std::string& uniform_name, const AmbientLightGpuData& data, const char* color_member = ".color")
		{
			const auto color_loc = GetUniformLocation(uniform_name + color_member);
			SetLight(color_loc, data);
		}
		void SetMandatoryLight(const std::string& uniform_name, const AmbientLightGpuData& data, const char* color_member = ".color")
		{
			const auto color_loc = GetMandatoryUniformLocation(uniform_name + color_member);
			SetLight(color_loc, data);
		}
		
		void SetOptionalLight(const std::string& uniform_name, const DirectionalLightGpuData& data, const char* direction_member = ".direction", const char* color_member = ".color")
		{
			const auto dir_loc = GetOptionalUniformLocation(uniform_name + direction_member);
			const auto color_loc = GetOptionalUniformLocation(uniform_name + color_member);
			if (-1 != color_loc && -1 != dir_loc) 
				SetLight(dir_loc, color_loc, data);
		}
		void SetLight(const std::string& uniform_name, const DirectionalLightGpuData& data, const char* direction_member = ".direction", const char* color_member = ".color")
		{
			const auto dir_loc = GetUniformLocation(uniform_name + direction_member);
			const auto color_loc = GetUniformLocation(uniform_name + color_member);
			SetLight(dir_loc, color_loc, data);
		}
		void SetMandatoryLight(const std::string& uniform_name, const DirectionalLightGpuData& data, const char* direction_member = ".direction", const char* color_member = ".color")
		{
			const auto dir_loc = GetMandatoryUniformLocation(uniform_name + direction_member);
			const auto color_loc = GetMandatoryUniformLocation(uniform_name + color_member);
			SetLight(dir_loc, color_loc, data);
		}

		void SetOptionalLight(const std::string& uniform_name, const PointLightGpuData& data, const char* position_member = ".position", const char* color_member = ".color", const char* attenuation_member = ".attenuation")
		{
			const auto pos_loc = GetOptionalUniformLocation(uniform_name + position_member);
			const auto color_loc = GetOptionalUniformLocation(uniform_name + color_member);
			const auto atten_loc = GetOptionalUniformLocation(uniform_name + attenuation_member);
			if (-1 != pos_loc && -1 != color_loc && -1 != atten_loc)
				SetLight(pos_loc, color_loc, atten_loc, data);
		}
		void SetLight(const std::string& uniform_name, const PointLightGpuData& data, const char* position_member = ".position", const char* color_member = ".color", const char* attenuation_member = ".attenuation")
		{
			const auto pos_loc = GetUniformLocation(uniform_name + position_member);
			const auto color_loc = GetUniformLocation(uniform_name + color_member);
			const auto atten_loc = GetUniformLocation(uniform_name + attenuation_member);
			SetLight(pos_loc, color_loc, atten_loc, data);
		}
		void SetMandatoryLight(const std::string& uniform_name, const PointLightGpuData& data, const char* position_member = ".position", const char* color_member = ".color", const char* attenuation_member = ".attenuation")
		{
			const auto pos_loc = GetMandatoryUniformLocation(uniform_name + position_member);
			const auto color_loc = GetMandatoryUniformLocation(uniform_name + color_member);
			const auto atten_loc = GetMandatoryUniformLocation(uniform_name + attenuation_member);
			SetLight(pos_loc, color_loc, atten_loc, data);
		}


	private:
		GLuint m_prog_handle;
		static std::string kNewLine;
		std::vector<std::string> m_vertex_shader_sources;
		std::vector<std::string> m_tess_control_shader_sources;
		std::vector<std::string> m_tess_eval_shader_sources;
		std::vector<std::string> m_geometry_shader_sources;
		std::vector<std::string> m_fragment_shader_sources;
		std::vector<std::string> m_compute_shader_sources;
		std::vector<std::tuple<GLuint, const GLchar*>> m_fragment_outputs;
		std::unordered_map<std::string, GLuint> m_uniform_locations;
		std::unordered_map<std::string, GLuint> m_uniform_block_indices;
		std::vector<const char*> m_transform_feedback_varyings;
		GLenum m_transform_feedback_buffer_mode;
		static const int kMaxShaders = 6;
		std::array<GLuint, kMaxShaders> m_shaderHandles;
		VertexAttribData m_vertex_attrib_config;
		GLenum m_kind_of_primitives;
		std::vector<std::tuple<std::string, AutoMatrix>> m_auto_matrices;
		std::vector<std::function<void()>> m_auto_mats_action_config;
		std::array<bool, 16> m_auto_mat_do_calc;
		std::array<glm::mat4, 16> m_auto_mat_action_cache;
		std::vector<std::function<void()>> m_auto_mat_calcers;
		GLuint m_sampler_auto_index;

#if defined(_DEBUG) && defined(FEATURE_NOT_READY_YET)
		std::function<void()> m_files_changed;
#endif
	};

	void Render(const Shader& shader, RenderConfig rnd_cfg, GLuint indices_len);
	void RenderMesh(const Shader& shader, Mesh& mesh);
	void RenderFullScreen(const Shader& shader);
	void RenderMeshes(const Shader& shader, const MeshRenderData& meshes_and_their_vaos);
	void RenderMeshesWithAlignedUniformSetters(const Shader& shader, const MeshRenderData& meshes_and_their_vaos, const MeshUniformSettersForShader& uniform_setters);
	void UnbindVAO();
	void UnbindShader();
}
