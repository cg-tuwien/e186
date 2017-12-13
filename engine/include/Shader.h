#pragma once

namespace e186
{
	class Shader
	{
		static GLuint Compile(const char* source, GLenum shaderType);
		void CheckErrorAndPrintInfoLog(const char* gl_error_location_hint, const char* info_log_description);
		void PrintInfoLog(const char* info_log_description);
		void DetermineTessData();
		void DetermineVertexAttribConfig();
		void DeterminePrimitivesMode();

		static std::string LoadFromFile(const std::string& path);

	public:
		Shader();
		Shader(const Shader& other) = delete;
		Shader(Shader&& other) noexcept;
		Shader& operator=(const Shader& other) = delete;
		Shader& operator=(Shader&& other) noexcept;
		~Shader();

		Shader& AddVertexShaderSource(std::string shaderSource);
		Shader& AddTesselationControlShaderSource(std::string shaderSource);
		Shader& AddTesselationEvaluationShaderSource(std::string shaderSource);
		Shader& AddGeometryShaderSource(std::string shaderSource);
		Shader& AddFragmentShaderSource(std::string shaderSource, std::vector<std::tuple<GLuint, const GLchar*>> outputs);
		Shader& AddComputeShaderSource(std::string shaderSource);
		Shader& AddVertexShaderSourceFromFile(std::string path);
		Shader& AddTesselationControlShaderSourceFromFile(std::string path);
		Shader& AddTesselationEvaluationShaderSourceFromFile(std::string path);
		Shader& AddGeometryShaderSourceFromFile(std::string path);
		Shader& AddFragmentShaderSourceFromFile(std::string path, std::vector<std::tuple<GLuint, const GLchar*>> outputs);
		Shader& AddComputeShaderSourceFromFile(std::string path);
		Shader& Build();
		Shader& QueryUniformLocations(const std::vector<std::string>& names);
		Shader& QueryUniformLocation(const std::string& name);
		Shader& Destroy();
		bool HasUniform(const std::string& name) const;
		GLuint GetUniformLocation(const std::string& name);
		explicit operator GLuint() const;
		GLuint handle() const;

		bool HasTesselationShaders() const;
		GLint patch_vertices() const;
		bool HasGeometryShader() const;
		unsigned int vertex_attrib_config() const;
		GLenum kind_of_primitives() const;
		void set_kind_of_primitives(GLenum mode);

		void Use() const 
		{
			glUseProgram(m_prog_handle);
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

		void SetSampler(GLuint location, const TexInfo& value, GLuint texture_unit = 0) const
		{
			check_gl_error("before SetSampler");
			glActiveTexture(static_cast<GLenum>(GL_TEXTURE0 + texture_unit));
			value.Bind();
			glUniform1i(location, static_cast<GLint>(texture_unit));
			check_gl_error("after SetSampler");
		}
			
		void SetImageTexture(GLuint location, const TexInfo& value, GLuint unit, GLint level, GLboolean layered, GLint layer, GLenum access)
		{
			glBindImageTexture(unit, value.handle(), level, layered, layer, access, value.internal_format());
			glUniform1i(location, static_cast<GLint>(unit));
		}
			
		template <typename... Args>
		void SetUniform(const std::string uniform_name, Args&&... args)
		{
			SetUniform(GetUniformLocation(uniform_name), std::forward<Args>(args)...);
		}

		template <typename... Args>
		void SetSampler(const std::string uniform_name, Args&&... args)
		{
			SetSampler(GetUniformLocation(uniform_name), std::forward<Args>(args)...);
		}

		template <typename... Args>
		void SetImageTexture(const std::string uniform_name, Args&&... args)
		{
			SetImageTexture(GetUniformLocation(uniform_name), std::forward<Args>(args)...);
		}

	private:
		GLuint m_prog_handle;
		std::string m_vertex_shader_source;
		std::string m_tess_control_shader_source;
		std::string m_tess_eval_shader_source;
		std::string m_geometry_shader_source;
		std::string m_fragment_shader_source;
		std::string m_compute_shader_source;
		std::vector<std::tuple<GLuint, const GLchar*>> m_fragment_outputs;
		std::unordered_map<std::string, GLuint> m_uniform_locations;
		GLint m_patch_vertices;
		static const int MAX_SHADERS = 6;
		GLuint m_shaderHandles[MAX_SHADERS];
		unsigned int m_vertex_attrib_config;
		GLenum m_kind_of_primitives;
	};

}
