#include "Shader.h"
#include <glad/glad.h>

namespace e186
{
	std::string Shader::LoadFromFile(const std::string& path)
	{
		std::ifstream stream(path, std::ifstream::in);
		if (!stream.good() || !stream || stream.fail())
		{
			throw ExceptionWithCallstack("Unable to load shader source from path: " + path);
		}
		std::string filecontents = std::string(std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>());
		if (filecontents.empty())
		{
			log_warning("Filecontents empty when loading shader from path[%s]", path.c_str());
		}
		return filecontents;
	}

	Shader::Shader()
		: m_prog_handle(0),
		m_vertex_shader_source(),
		m_tess_control_shader_source(),
		m_tess_eval_shader_source(),
		m_geometry_shader_source(),
		m_fragment_shader_source(),
		m_compute_shader_source(),
		m_fragment_outputs(),
		m_uniform_locations(),
		m_patch_vertices(0),
		m_vertex_attrib_config(VertexAttribData_Nothing),
		m_kind_of_primitives(GL_TRIANGLES)
	{
		for (int i = 0; i < kMaxShaders; ++i)
		{
			m_shaderHandles[i] = 0;
		}
	}

	Shader::Shader(Shader&& other) noexcept
		: m_prog_handle(other.m_prog_handle),
		m_vertex_shader_source(std::move(other.m_vertex_shader_source)),
		m_tess_control_shader_source(std::move(other.m_tess_control_shader_source)),
		m_tess_eval_shader_source(std::move(other.m_tess_eval_shader_source)),
		m_geometry_shader_source(std::move(other.m_geometry_shader_source)),
		m_fragment_shader_source(std::move(other.m_fragment_shader_source)),
		m_compute_shader_source(std::move(other.m_compute_shader_source)),
		m_fragment_outputs(std::move(other.m_fragment_outputs)),
		m_uniform_locations(std::move(other.m_uniform_locations)),
		m_patch_vertices(other.m_patch_vertices),
		m_vertex_attrib_config(other.m_vertex_attrib_config),
		m_kind_of_primitives(other.m_kind_of_primitives)
	{
		other.m_prog_handle = 0;
		other.m_patch_vertices = 0;
		other.m_vertex_attrib_config = VertexAttribData_Nothing;
		for (int i = 0; i < kMaxShaders; ++i)
		{
			m_shaderHandles[i] = other.m_shaderHandles[i];
			other.m_shaderHandles[i] = 0;
		}
		log_debug("Move constructing Shader with m_prog_handle[%u]", m_prog_handle);
	}

	Shader& Shader::operator=(Shader&& other) noexcept
	{
		m_prog_handle = other.m_prog_handle;
		other.m_prog_handle = 0;

		m_patch_vertices = other.m_patch_vertices;
		other.m_patch_vertices = 0;

		m_vertex_attrib_config = other.m_vertex_attrib_config;
		other.m_vertex_attrib_config = VertexAttribData_Nothing;

		m_kind_of_primitives = other.m_kind_of_primitives;

		for (int i = 0; i < kMaxShaders; ++i)
		{
			m_shaderHandles[i] = other.m_shaderHandles[i];
			other.m_shaderHandles[i] = 0;
		}

		m_vertex_shader_source = std::move(other.m_vertex_shader_source);
		m_tess_control_shader_source = std::move(other.m_tess_control_shader_source);
		m_tess_eval_shader_source = std::move(other.m_tess_eval_shader_source);
		m_geometry_shader_source = std::move(other.m_geometry_shader_source);
		m_fragment_shader_source = std::move(other.m_fragment_shader_source);
		m_compute_shader_source = std::move(other.m_compute_shader_source);
		m_fragment_outputs = std::move(other.m_fragment_outputs);
		m_uniform_locations = std::move(other.m_uniform_locations);

		log_debug("Move assigning Shader with m_prog_handle[%u]", m_prog_handle);
		return *this;
	}


	Shader::~Shader()
	{
		if (0 != m_prog_handle)
		{
			log_debug("Destructing Shader with m_prog_handle[%u]", m_prog_handle);
		}
		Destroy();
	}

	Shader& Shader::AddVertexShaderSourceFromFile(std::string path)
	{
		return AddVertexShaderSource(LoadFromFile(path));
	}

	Shader& Shader::AddTesselationControlShaderSourceFromFile(std::string path)
	{
		return AddTesselationControlShaderSource(LoadFromFile(path));
	}

	Shader& Shader::AddTesselationEvaluationShaderSourceFromFile(std::string path)
	{
		return AddTesselationEvaluationShaderSource(LoadFromFile(path));
	}

	Shader& Shader::AddGeometryShaderSourceFromFile(std::string path)
	{
		return AddGeometryShaderSource(LoadFromFile(path));
	}

	Shader& Shader::AddFragmentShaderSourceFromFile(std::string path, std::vector<std::tuple<GLuint, const GLchar*>> outputs)
	{
		return AddFragmentShaderSource(LoadFromFile(path), outputs);
	}

	Shader& Shader::AddComputeShaderSourceFromFile(std::string path)
	{
		return AddComputeShaderSource(LoadFromFile(path));
	}

	Shader& Shader::SetTransformFeedbackVaryings(std::vector<const char*> varyings, GLenum buffer_mode)
	{
		m_transform_feedback_varyings = std::move(varyings);
		m_transform_feedback_buffer_mode = buffer_mode;
		return *this;
	}

	Shader& Shader::AddVertexShaderSource(std::string shaderSource)
	{
		m_vertex_shader_source = std::move(shaderSource);
		return *this;
	}

	Shader& Shader::AddTesselationControlShaderSource(std::string shaderSource)
	{
		m_tess_control_shader_source = std::move(shaderSource);
		return *this;
	}

	Shader& Shader::AddTesselationEvaluationShaderSource(std::string shaderSource)
	{
		m_tess_eval_shader_source = std::move(shaderSource);
		return *this;
	}

	Shader& Shader::AddGeometryShaderSource(std::string shaderSource)
	{
		m_geometry_shader_source = std::move(shaderSource);
		return *this;
	}

	Shader& Shader::AddFragmentShaderSource(std::string shaderSource, std::vector<std::tuple<GLuint, const GLchar*>> outputs)
	{
		m_fragment_shader_source = std::move(shaderSource);
		m_fragment_outputs = std::move(outputs);
		return *this;
	}

	Shader& Shader::AddComputeShaderSource(std::string shaderSource)
	{
		m_compute_shader_source = std::move(shaderSource);
		return *this;
	}

	bool Shader::HasTesselationShaders() const
	{
		return 0 != m_shaderHandles[1] && 0 != m_shaderHandles[2];
	}

	GLint Shader::patch_vertices() const
	{
		return m_patch_vertices;
	}

	unsigned int Shader::vertex_attrib_config() const
	{
		return m_vertex_attrib_config;
	}

	GLenum Shader::kind_of_primitives() const
	{
		return m_kind_of_primitives;
	}

	void Shader::set_kind_of_primitives(GLenum mode)
	{
		m_kind_of_primitives = mode;
	}

	bool Shader::HasGeometryShader() const
	{
		return 0 != m_shaderHandles[3];
	}

	void Shader::DetermineTessData()
	{
		if (HasTesselationShaders())
		{
			glGetIntegerv(GL_PATCH_VERTICES, &m_patch_vertices);
		}
	}

	void Shader::DetermineVertexAttribConfig()
	{
		GLchar name[GL_ACTIVE_ATTRIBUTE_MAX_LENGTH];
		*name = 0;
		GLsizei name_len = 0;
		GLint size_of_the_attribute_variable = 0;
		GLenum type_of_the_attribute_variable = 0;

		unsigned int attrib_config = VertexAttribData_Nothing;

		bool has_bone_weights = false;
		bool has_bone_indices = false;
		bool has_tangents = false;
		bool has_bitangents = false;

		GLint count;
		glGetProgramiv(handle(), GL_ACTIVE_ATTRIBUTES, &count);
		for (int i = 0; i < count; ++i)
		{
			name_len = 0;
			glGetActiveAttrib(handle(), i, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &name_len, &size_of_the_attribute_variable, &type_of_the_attribute_variable, name);
			auto location = glGetAttribLocation(handle(), name);
			if (is_valid_VertexAttribLocation(location))
			{
				switch (static_cast<VertexAttribLocation>(location))
				{
				case VertexAttribLocation::Position:
					attrib_config = attrib_config | VertexAttribData_Position;
					log_debug_verbose("For shader with handle[%u], positions attribute is named [%s]", handle(), name);
					break;
				case VertexAttribLocation::TexCoord:
					attrib_config = attrib_config | VertexAttribData_Tex2D;
					log_debug_verbose("For shader with handle[%u], tex-coords attribute is named [%s]", handle(), name);
					break;
				case VertexAttribLocation::Normal:
					attrib_config = attrib_config | VertexAttribData_Normal;
					log_debug_verbose("For shader with handle[%u], normals attribute is named [%s]", handle(), name);
					break;
				case VertexAttribLocation::Color:
					attrib_config = attrib_config | VertexAttribData_Color;
					log_debug_verbose("For shader with handle[%u], colors attribute is named [%s]", handle(), name);
					break;
				case VertexAttribLocation::TexCoord2:
					attrib_config = attrib_config | VertexAttribData_Tex2D_2;
					log_debug_verbose("For shader with handle[%u], tex-coords2 attribute is named [%s]", handle(), name);
					break;
				case VertexAttribLocation::TexCoord3:
					attrib_config = attrib_config | VertexAttribData_Tex2D_3;
					log_debug_verbose("For shader with handle[%u], tex-coords3 attribute is named [%s]", handle(), name);
					break;
				case VertexAttribLocation::BoneWeights:
					has_bone_weights = true;
					log_debug_verbose("For shader with handle[%u], bone-weights attribute is named [%s]", handle(), name);
					break;
				case VertexAttribLocation::BoneIndices:
					has_bone_indices = true;
					log_debug_verbose("For shader with handle[%u], bone-indices attribute is named [%s]\n  => has both bone attributes", handle(), name);
					break;
				case VertexAttribLocation::Tangents:
					has_tangents = true;
					log_debug_verbose("For shader with handle[%u], tangents attribute is named [%s]", handle(), name);
					break;
				case VertexAttribLocation::Bitangents:
					has_bitangents = true;
					log_debug_verbose("For shader with handle[%u], bitangents attribute is named [%s]\n  => has both tangent space attributes", handle(), name);
					break;
				case VertexAttribLocation::Param0:
					attrib_config = attrib_config | VertexAttribData_Param0;
					log_debug_verbose("For shader with handle[%u], param0 attribute is named [%s]", handle(), name);
					break;
				case VertexAttribLocation::Param1:
					attrib_config = attrib_config | VertexAttribData_Param1;
					log_debug_verbose("For shader with handle[%u], param1 attribute is named [%s]", handle(), name);
					break;
				case VertexAttribLocation::Param2:
					attrib_config = attrib_config | VertexAttribData_Param2;
					log_debug_verbose("For shader with handle[%u], param2 attribute is named [%s]", handle(), name);
					break;
				case VertexAttribLocation::Param3:
					attrib_config = attrib_config | VertexAttribData_Param3;
					log_debug_verbose("For shader with handle[%u], param3 attribute is named [%s]", handle(), name);
					break;
				case VertexAttribLocation::Param4:
					attrib_config = attrib_config | VertexAttribData_Param4;
					log_debug_verbose("For shader with handle[%u], param4 attribute is named [%s]", handle(), name);
					break;
				case VertexAttribLocation::Param5:
					attrib_config = attrib_config | VertexAttribData_Param5;
					log_debug_verbose("For shader with handle[%u], param5 attribute is named [%s]", handle(), name);
					break;
				}
			}
		}

		if (has_bone_weights && has_bone_indices)
		{
			attrib_config = attrib_config | VertexAttribData_Bones;
		}

		if (has_tangents || has_bitangents)
		{
			attrib_config = attrib_config | VertexAttribData_TangentSpace;
		}

		m_vertex_attrib_config = attrib_config;
	}

	void Shader::DeterminePrimitivesMode()
	{
		if (HasTesselationShaders())
		{
			m_kind_of_primitives = GL_PATCHES;
		}
		else
		{
			m_kind_of_primitives = GL_TRIANGLES;
		}
	}

	Shader& Shader::Build()
	{
		m_shaderHandles[0] = m_vertex_shader_source.empty()       ? 0 : Compile(m_vertex_shader_source.c_str(),       GL_VERTEX_SHADER);
		m_shaderHandles[1] = m_tess_control_shader_source.empty() ? 0 : Compile(m_tess_control_shader_source.c_str(), GL_TESS_CONTROL_SHADER);
		m_shaderHandles[2] = m_tess_eval_shader_source.empty()    ? 0 : Compile(m_tess_eval_shader_source.c_str(),    GL_TESS_EVALUATION_SHADER);
		m_shaderHandles[3] = m_geometry_shader_source.empty()     ? 0 : Compile(m_geometry_shader_source.c_str(),     GL_GEOMETRY_SHADER);
		m_shaderHandles[4] = m_fragment_shader_source.empty()     ? 0 : Compile(m_fragment_shader_source.c_str(),     GL_FRAGMENT_SHADER);
		m_shaderHandles[5] = m_compute_shader_source.empty()      ? 0 : Compile(m_compute_shader_source.c_str(),      GL_COMPUTE_SHADER);

		const auto progHandle = glCreateProgram();

		for (int i = 0; i < kMaxShaders; ++i)
		{
			if (0 == m_shaderHandles[i])
			{
				continue;
			}
			glAttachShader(progHandle, m_shaderHandles[i]);
			CheckErrorAndPrintInfoLog("Shader::Build after glAttachShader", "Could not attach shader");
		}

		if (0 != m_shaderHandles[4])
		{
			// before linking, define outputs of fragment-shader
			for (auto const& outp : m_fragment_outputs)
			{
				auto color = std::get<0>(outp);
				auto name = std::get<1>(outp);
				glBindFragDataLocation(progHandle, color, name);
				CheckErrorAndPrintInfoLog((std::string("Shader::Build after output location for ") + name).c_str(), "Could not bind fragment data location");
			}
		}
		else
		{
			log_warning("Shader Handle for fragment shader is 0");
		}

		// Don't define attribute locations - set them im shader files using layout (location = 0)
		/*for (auto const& attrLoc : attribLocations)
		{
		auto index = std::get<0>(attrLoc);
		auto name = std::get<1>(attrLoc);
		glBindAttribLocation(progHandle, index, name);
		check_gl_error((std::string("fu.shader.Build at attribute location for ") + name).c_str());
		}*/

		if (m_transform_feedback_varyings.size() > 0)
		{
			glTransformFeedbackVaryings(progHandle, static_cast<GLsizei>(m_transform_feedback_varyings.size()), &m_transform_feedback_varyings[0], m_transform_feedback_buffer_mode);
		}

		glLinkProgram(progHandle);
		CheckErrorAndPrintInfoLog("Shader::Build after glLinkProgram", "Could not link program");

		GLint linkSuccess;
		glGetProgramiv(progHandle, GL_LINK_STATUS, &linkSuccess);

		if (linkSuccess == GL_FALSE)
		{
			PrintInfoLog("Could not link program");

			Destroy();
			throw ExceptionWithCallstack("Linking shader program failed");
		}

		// success!
		m_prog_handle = progHandle;
		DetermineVertexAttribConfig();
		DetermineTessData();
		DeterminePrimitivesMode();
		return *this;
	}

	Shader& Shader::QueryUniformLocations(const std::vector<std::string>& names)
	{
		if (0 == m_prog_handle)
		{
			throw ExceptionWithCallstack("QueryUniformLocations is useless since the program handle is 0");
		}

		glUseProgram(m_prog_handle);
		for (auto const& name : names)
		{
			m_uniform_locations[name] = glGetUniformLocation(m_prog_handle, name.c_str());
		}
		return *this;
	}
		
	Shader& Shader::QueryUniformLocation(const std::string& name)
	{
		if (0 == m_prog_handle)
		{
			throw ExceptionWithCallstack("QueryUniformLocations is useless since the program handle is 0");
		}

		glUseProgram(m_prog_handle);
		auto loc = glGetUniformLocation(m_prog_handle, name.c_str());
		if (-1 == loc)
		{
			log_debug("Uniform %s not found.", name.c_str());
			//throw ExceptionWithCallstack("Uniform '" + name + "' not found.");
		}
		m_uniform_locations[name] = loc;
		return *this;
	}

	Shader& Shader::Destroy()
	{
		if (0 == m_prog_handle)
		{
			return *this;
		}

		GLsizei count = 0;
		GLuint attachedShaders[8]; // fragment, vertex, geometry, hull, domain => max. 5 aligned to 8
		glGetAttachedShaders(m_prog_handle, 8, &count, attachedShaders);

		// also delete the attached shaders
		for (int i = 0; i < count; i++)
		{
			glDeleteShader(attachedShaders[i]);
		}

		glDeleteProgram(m_prog_handle);
		m_prog_handle = 0;

		return *this;
	}

	bool Shader::HasUniform(const std::string& name) const
	{
		return m_uniform_locations.find(name) != m_uniform_locations.end();
	}

	GLuint Shader::GetUniformLocation(const std::string& name)
	{
		auto loc = m_uniform_locations.find(name);
		if (loc == m_uniform_locations.end())
		{
			QueryUniformLocation(name);
			return m_uniform_locations.at(name);
		}
		return loc->second;
	}

	Shader::operator GLuint() const
	{
		return m_prog_handle;
	}

	GLuint Shader::handle() const
	{
		return m_prog_handle;
	}

	void Shader::CheckErrorAndPrintInfoLog(const char* gl_error_location_hint, const char* info_log_description)
	{
		if (check_gl_error(gl_error_location_hint))
		{
			PrintInfoLog(info_log_description);
		}
	}

	void Shader::PrintInfoLog(const char* info_log_description)
	{
		GLint infoLen = 0;
		glGetProgramiv(handle(), GL_INFO_LOG_LENGTH, &infoLen);

		if (infoLen)
		{
			char* buf = new char[infoLen];
			glGetProgramInfoLog(handle(), infoLen, nullptr, buf);
			log_error("%s shader[%u]\n    Reason: %s\n", info_log_description, handle(), buf);
			delete[] buf;
		}
	}

	GLuint Shader::Compile(const char* source, GLenum shaderType)
	{
		if (!source)
		{
			return GLuint(0);
		}

		auto shaderHandle = glCreateShader(shaderType);
		if (!shaderHandle)
		{
			return GLuint(0);
		}

		glShaderSource(shaderHandle, 1, &source, 0);
		glCompileShader(shaderHandle);

		GLint compileSuccess;
		glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &compileSuccess);

		if (compileSuccess == GL_FALSE)
		{
			GLint infoLen = 0;
			glGetShaderiv(shaderHandle, GL_INFO_LOG_LENGTH, &infoLen);

			if (infoLen) 
			{
				char* buf = new char[infoLen];
				glGetShaderInfoLog(shaderHandle, infoLen, nullptr, buf);
				log_error((std::string("Could not compile ") + (
					(shaderType == GL_VERTEX_SHADER) ? "vertex" :
					(shaderType == GL_TESS_CONTROL_SHADER) ? "tesselation control" :
					(shaderType == GL_TESS_EVALUATION_SHADER) ? "tesselation evaluation" :
					(shaderType == GL_GEOMETRY_SHADER) ? "geometry" :
					(shaderType == GL_FRAGMENT_SHADER) ? "fragment" :
					(shaderType == GL_COMPUTE_SHADER) ? "compute" : "?") + " shader with handle[%u]\n    Reason: %s\n").c_str(), shaderHandle, buf);
				delete[] buf;
			}

			glDeleteShader(shaderHandle);

			throw ExceptionWithCallstack(std::string("Compiling ") + (
				(shaderType == GL_VERTEX_SHADER) ? "vertex shader" :
				(shaderType == GL_TESS_CONTROL_SHADER) ? "tesselation control" :
				(shaderType == GL_TESS_EVALUATION_SHADER) ? "tesselation evaluation" :
				(shaderType == GL_GEOMETRY_SHADER) ? "geometry shader" :
				(shaderType == GL_FRAGMENT_SHADER) ? "fragment shader" :
				(shaderType == GL_COMPUTE_SHADER) ? "compute shader" : "?") + " failed");
		}

		return shaderHandle;
	}

	void RenderVAO(const Shader& shader, VAOType vao, GLuint indices_len)
	{
		GLenum mode = shader.kind_of_primitives();
		if (GL_PATCHES == mode)
		{
			glPatchParameteri(GL_PATCH_VERTICES, shader.patch_vertices());
		}
		glBindVertexArray(vao);
		glDrawElements(mode, indices_len, GL_UNSIGNED_INT, nullptr);
	}

	void RenderMesh(const Shader& shader, Mesh& mesh)
	{
		RenderVAO(shader, Mesh::GetOrCreateVAOForShader(mesh, shader), mesh.indices_length());
	}

	void RenderMeshes(const Shader& shader, const std::vector<std::tuple<MeshRef, VAOType>>& meshes_and_their_vaos)
	{
		GLenum mode = shader.kind_of_primitives();
		if (GL_PATCHES == mode)
		{
			glPatchParameteri(GL_PATCH_VERTICES, shader.patch_vertices());
		}

		for (auto& tupl : meshes_and_their_vaos)
		{
			Mesh& mesh = std::get<0>(tupl);
			VAOType vao = std::get<1>(tupl);

			glBindVertexArray(vao);
			glDrawElements(mode, mesh.indices_length(), GL_UNSIGNED_INT, nullptr);
		}
	}

	void RenderMeshesWithAlignedUniformSetters(const Shader& shader, const std::vector<std::tuple<MeshRef, VAOType>>& meshes_and_their_vaos, const std::vector<std::tuple<MeshRef, UniformSetter>>& uniform_setters)
	{
		GLenum mode = shader.kind_of_primitives();
		if (GL_PATCHES == mode)
		{
			glPatchParameteri(GL_PATCH_VERTICES, shader.patch_vertices());
		}

		assert(meshes_and_their_vaos.size() == uniform_setters.size());
		auto n = meshes_and_their_vaos.size();
		for (auto i = 0; i < n; ++i)
		{
			auto& tupl = meshes_and_their_vaos[i];
			Mesh& mesh = std::get<0>(tupl);
			assert(&mesh == &static_cast<Mesh&>(std::get<0>(uniform_setters[i])));
			VAOType vao = std::get<1>(tupl);
			std::get<1>(uniform_setters[i])(shader, *mesh.material_data());

			glBindVertexArray(vao);
			glDrawElements(mode, mesh.indices_length(), GL_UNSIGNED_INT, nullptr);
		}
	}

	void UnbindVAO()
	{
		glBindVertexArray(0);
	}
}
