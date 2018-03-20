#include "Shader.h"
#include <glad/glad.h>
#include <sstream>

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
		m_vertex_shader_sources(),
		m_tess_control_shader_sources(),
		m_tess_eval_shader_sources(),
		m_geometry_shader_sources(),
		m_fragment_shader_sources(),
		m_compute_shader_sources(),
		m_fragment_outputs(),
		m_uniform_locations(),
		m_transform_feedback_varyings(),
		m_transform_feedback_buffer_mode(0),
		m_patch_vertices(0),
		m_shaderHandles { 0, 0, 0, 0, 0, 0, },
		m_vertex_attrib_config(VertexAttribData::Nothing),
		m_kind_of_primitives(GL_TRIANGLES),
		m_auto_matrices(),
		m_auto_mats_action_config(),
		m_auto_mat_do_calc
		{
			false,  // 00 AutoMatrix::Nothing
			false,  // 01 AutoMatrix::TransformMatrix	
			false, 	// 02 AutoMatrix::ModelMatrix		
			false, 	// 03 AutoMatrix::TransformMatrix | AutoMatrix::ModelMatrix
			false, 	// 04 AutoMatrix::ViewMatrix		
			false, 	// 05 AutoMatrix::TransformMatrix | AutoMatrix::ViewMatrix
			false, 	// 06 AutoMatrix::ModelMatrix | AutoMatrix::ViewMatrix
			false, 	// 07 AutoMatrix::TransformMatrix | AutoMatrix::ModelMatrix | AutoMatrix::ViewMatrix
			false, 	// 08 AutoMatrix::ProjectionMatrix
			false, 	// 09 AutoMatrix::TransformMatrix | AutoMatrix::ProjectionMatrix
			false, 	// 10 AutoMatrix::ModelMatrix | AutoMatrix::ProjectionMatrix
			false, 	// 11 AutoMatrix::TransformMatrix | AutoMatrix::ModelMatrix | AutoMatrix::ProjectionMatrix
			false, 	// 12 AutoMatrix::ViewMatrix | AutoMatrix::ProjectionMatrix
			false, 	// 13 AutoMatrix::TransformMatrix | AutoMatrix::ViewMatrix | AutoMatrix::ProjectionMatrix
			false, 	// 14 AutoMatrix::ModelMatrix | AutoMatrix::ViewMatrix | AutoMatrix::ProjectionMatrix
			false   // 15 AutoMatrix::TransformMatrix | AutoMatrix::ModelMatrix | AutoMatrix::ViewMatrix | AutoMatrix::ProjectionMatrix
		},
		m_auto_mat_action_cache 
		{
			glm::mat4(1.0), // 00 AutoMatrix::Nothing
			glm::mat4(1.0), // 01 AutoMatrix::TransformMatrix	
			glm::mat4(1.0),	// 02 AutoMatrix::ModelMatrix		
			glm::mat4(1.0),	// 03 AutoMatrix::TransformMatrix | AutoMatrix::ModelMatrix
			glm::mat4(1.0),	// 04 AutoMatrix::ViewMatrix		
			glm::mat4(1.0),	// 05 AutoMatrix::TransformMatrix | AutoMatrix::ViewMatrix
			glm::mat4(1.0),	// 06 AutoMatrix::ModelMatrix | AutoMatrix::ViewMatrix
			glm::mat4(1.0),	// 07 AutoMatrix::TransformMatrix | AutoMatrix::ModelMatrix | AutoMatrix::ViewMatrix
			glm::mat4(1.0),	// 08 AutoMatrix::ProjectionMatrix
			glm::mat4(1.0),	// 09 AutoMatrix::TransformMatrix | AutoMatrix::ProjectionMatrix
			glm::mat4(1.0),	// 10 AutoMatrix::ModelMatrix | AutoMatrix::ProjectionMatrix
			glm::mat4(1.0),	// 11 AutoMatrix::TransformMatrix | AutoMatrix::ModelMatrix | AutoMatrix::ProjectionMatrix
			glm::mat4(1.0),	// 12 AutoMatrix::ViewMatrix | AutoMatrix::ProjectionMatrix
			glm::mat4(1.0),	// 13 AutoMatrix::TransformMatrix | AutoMatrix::ViewMatrix | AutoMatrix::ProjectionMatrix
			glm::mat4(1.0),	// 14 AutoMatrix::ModelMatrix | AutoMatrix::ViewMatrix | AutoMatrix::ProjectionMatrix
			glm::mat4(1.0)	// 15 AutoMatrix::TransformMatrix | AutoMatrix::ModelMatrix | AutoMatrix::ViewMatrix | AutoMatrix::ProjectionMatrix
		},
		m_auto_mat_calcers()
	{
	}

	Shader::Shader(Shader&& other) noexcept
		: m_prog_handle(other.m_prog_handle),
		m_vertex_shader_sources(std::move(other.m_vertex_shader_sources)),
		m_tess_control_shader_sources(std::move(other.m_tess_control_shader_sources)),
		m_tess_eval_shader_sources(std::move(other.m_tess_eval_shader_sources)),
		m_geometry_shader_sources(std::move(other.m_geometry_shader_sources)),
		m_fragment_shader_sources(std::move(other.m_fragment_shader_sources)),
		m_compute_shader_sources(std::move(other.m_compute_shader_sources)),
		m_fragment_outputs(std::move(other.m_fragment_outputs)),
		m_uniform_locations(std::move(other.m_uniform_locations)),
		m_transform_feedback_varyings(std::move(other.m_transform_feedback_varyings)),
		m_transform_feedback_buffer_mode(std::move(other.m_transform_feedback_buffer_mode)),
		m_patch_vertices(other.m_patch_vertices),
		m_shaderHandles(std::move(other.m_shaderHandles)),
		m_vertex_attrib_config(other.m_vertex_attrib_config),
		m_kind_of_primitives(other.m_kind_of_primitives),
		m_auto_matrices(std::move(other.m_auto_matrices)),
		m_auto_mats_action_config(std::move(other.m_auto_mats_action_config)),
		m_auto_mat_do_calc(std::move(other.m_auto_mat_do_calc)),
		m_auto_mat_action_cache(std::move(other.m_auto_mat_action_cache)),
		m_auto_mat_calcers(std::move(other.m_auto_mat_calcers))
	{
		other.m_prog_handle = 0;
		other.m_patch_vertices = 0;
		other.m_vertex_attrib_config = VertexAttribData::Nothing;
		other.m_transform_feedback_buffer_mode = 0;
		log_debug("Move constructing Shader with m_prog_handle[%u]", m_prog_handle);
	}

	Shader& Shader::operator=(Shader&& other) noexcept
	{
		m_prog_handle = other.m_prog_handle;
		other.m_prog_handle = 0;

		m_patch_vertices = other.m_patch_vertices;
		other.m_patch_vertices = 0;

		m_vertex_attrib_config = other.m_vertex_attrib_config;
		other.m_vertex_attrib_config = VertexAttribData::Nothing;

		m_transform_feedback_buffer_mode = other.m_transform_feedback_buffer_mode;
		other.m_transform_feedback_buffer_mode = 0;

		m_kind_of_primitives = other.m_kind_of_primitives;

		m_vertex_shader_sources = std::move(other.m_vertex_shader_sources);
		m_tess_control_shader_sources = std::move(other.m_tess_control_shader_sources);
		m_tess_eval_shader_sources = std::move(other.m_tess_eval_shader_sources);
		m_geometry_shader_sources = std::move(other.m_geometry_shader_sources);
		m_fragment_shader_sources = std::move(other.m_fragment_shader_sources);
		m_compute_shader_sources = std::move(other.m_compute_shader_sources);
		m_fragment_outputs = std::move(other.m_fragment_outputs);
		m_uniform_locations = std::move(other.m_uniform_locations); 
		m_transform_feedback_varyings = std::move(other.m_transform_feedback_varyings);
		m_shaderHandles = std::move(other.m_shaderHandles);
		m_auto_matrices = std::move(other.m_auto_matrices);
		m_auto_mats_action_config = std::move(other.m_auto_mats_action_config);
		m_auto_mat_do_calc = std::move(other.m_auto_mat_do_calc);
		m_auto_mat_action_cache = std::move(other.m_auto_mat_action_cache);
		m_auto_mat_calcers = std::move(other.m_auto_mat_calcers);

		log_debug("Move assigning Shader with m_prog_handle[%u]", m_prog_handle);
		return *this;
	}


	Shader::~Shader()
	{
#if defined(_DEBUG) && defined(FEATURE_NOT_READY_YET)
		Engine::current()->StopFileChangeNotifyCallbacks(&m_files_changed);
#endif
		if (0 != m_prog_handle)
		{
			log_debug("Destructing Shader with m_prog_handle[%u]", m_prog_handle);
		}
		Destroy();
	}




	void Shader::DetermineWhichAutoMatsToCalc()
	{
		for (const auto& tpl : m_auto_matrices)
		{
			auto matidx = static_cast<uint8_t>(std::get<1>(tpl)) & 0xF;
			m_auto_mat_do_calc[matidx] = true;
		}
	}

	void Shader::CreateAutoMatCalcers()
	{
		if (m_auto_matrices.size() == 0)
		{
			return;
		}

		// 00 AutoMatrix::Nothing
		// 01 AutoMatrix::TransformMatrix	
		// 02 AutoMatrix::ModelMatrix		
		// 04 AutoMatrix::ViewMatrix		
		// 08 AutoMatrix::ProjectionMatrix

		// 03 AutoMatrix::TransformMatrix | AutoMatrix::ModelMatrix
		if (m_auto_mat_do_calc[3]) {
			m_auto_mat_calcers.push_back([this]() {
				m_auto_mat_action_cache[3] = m_auto_mat_action_cache[2] * m_auto_mat_action_cache[1];
			});
		}
		// 05 AutoMatrix::TransformMatrix | AutoMatrix::ViewMatrix
		if (m_auto_mat_do_calc[5]) {
			m_auto_mat_calcers.push_back([this]() {
				m_auto_mat_action_cache[5] = m_auto_mat_action_cache[4] * m_auto_mat_action_cache[1];
			});
		}
		// 06 AutoMatrix::ModelMatrix | AutoMatrix::ViewMatrix
		if (m_auto_mat_do_calc[6]) {
			m_auto_mat_calcers.push_back([this]() {
				m_auto_mat_action_cache[6] = m_auto_mat_action_cache[4] * m_auto_mat_action_cache[2];
			});
		}
		// 09 AutoMatrix::TransformMatrix | AutoMatrix::ProjectionMatrix
		if (m_auto_mat_do_calc[9]) {
			m_auto_mat_calcers.push_back([this]() {
				m_auto_mat_action_cache[9] = m_auto_mat_action_cache[8] * m_auto_mat_action_cache[1];
			});
		}
		// 10 AutoMatrix::ModelMatrix | AutoMatrix::ProjectionMatrix
		if (m_auto_mat_do_calc[10]) {
			m_auto_mat_calcers.push_back([this]() {
				m_auto_mat_action_cache[10] = m_auto_mat_action_cache[8] * m_auto_mat_action_cache[2];
			});
		}
		// 12 AutoMatrix::ViewMatrix | AutoMatrix::ProjectionMatrix
		if (m_auto_mat_do_calc[12]) {
			m_auto_mat_calcers.push_back([this]() {
				m_auto_mat_action_cache[12] = m_auto_mat_action_cache[8] * m_auto_mat_action_cache[4];
			});
		}

		// 07 AutoMatrix::TransformMatrix | AutoMatrix::ModelMatrix | AutoMatrix::ViewMatrix
		if (m_auto_mat_do_calc[7]) {
			if (m_auto_mat_do_calc[3]) {
				m_auto_mat_calcers.push_back([this]() {
					m_auto_mat_action_cache[7] = m_auto_mat_action_cache[4] * m_auto_mat_action_cache[3];
				});
			}
			else if (m_auto_mat_do_calc[6]) {
				m_auto_mat_calcers.push_back([this]() {
					m_auto_mat_action_cache[7] = m_auto_mat_action_cache[6] * m_auto_mat_action_cache[1];
				});
			}
			else {
				m_auto_mat_calcers.push_back([this]() {
					m_auto_mat_action_cache[7] = m_auto_mat_action_cache[4] * m_auto_mat_action_cache[2] * m_auto_mat_action_cache[1];
				});
			}
		}
		// 11 AutoMatrix::TransformMatrix | AutoMatrix::ModelMatrix | AutoMatrix::ProjectionMatrix
		if (m_auto_mat_do_calc[11]) {
			if (m_auto_mat_do_calc[3]) {
				m_auto_mat_calcers.push_back([this]() {
					m_auto_mat_action_cache[11] = m_auto_mat_action_cache[8] * m_auto_mat_action_cache[3];
				});
			}
			else if (m_auto_mat_do_calc[10]) {
				m_auto_mat_calcers.push_back([this]() {
					m_auto_mat_action_cache[11] = m_auto_mat_action_cache[10] * m_auto_mat_action_cache[1];
				});
			}
			else {
				m_auto_mat_calcers.push_back([this]() {
					m_auto_mat_action_cache[11] = m_auto_mat_action_cache[8] * m_auto_mat_action_cache[2] * m_auto_mat_action_cache[1];
				});
			}
		}
		// 13 AutoMatrix::TransformMatrix | AutoMatrix::ViewMatrix | AutoMatrix::ProjectionMatrix
		if (m_auto_mat_do_calc[13]) {
			if (m_auto_mat_do_calc[5]) {
				m_auto_mat_calcers.push_back([this]() {
					m_auto_mat_action_cache[13] = m_auto_mat_action_cache[8] * m_auto_mat_action_cache[5];
				});
			}
			else if (m_auto_mat_do_calc[12]) {
				m_auto_mat_calcers.push_back([this]() {
					m_auto_mat_action_cache[13] = m_auto_mat_action_cache[12] * m_auto_mat_action_cache[1];
				});
			}
			else {
				m_auto_mat_calcers.push_back([this]() {
					m_auto_mat_action_cache[13] = m_auto_mat_action_cache[8] * m_auto_mat_action_cache[4] * m_auto_mat_action_cache[1];
				});
			}
		}
		// 14 AutoMatrix::ModelMatrix | AutoMatrix::ViewMatrix | AutoMatrix::ProjectionMatrix
		if (m_auto_mat_do_calc[14]) {
			if (m_auto_mat_do_calc[6]) {
				m_auto_mat_calcers.push_back([this]() {
					m_auto_mat_action_cache[14] = m_auto_mat_action_cache[8] * m_auto_mat_action_cache[6];
				});
			}
			else if (m_auto_mat_do_calc[12]) {
				m_auto_mat_calcers.push_back([this]() {
					m_auto_mat_action_cache[14] = m_auto_mat_action_cache[12] * m_auto_mat_action_cache[2];
				});
			}
			else {
				m_auto_mat_calcers.push_back([this]() {
					m_auto_mat_action_cache[14] = m_auto_mat_action_cache[8] * m_auto_mat_action_cache[4] * m_auto_mat_action_cache[2];
				});
			}
		}

		// 15 AutoMatrix::TransformMatrix | AutoMatrix::ModelMatrix | AutoMatrix::ViewMatrix | AutoMatrix::ProjectionMatrix
		if (m_auto_mat_do_calc[15]) {
			if (m_auto_mat_do_calc[7]) {
				m_auto_mat_calcers.push_back([this]() {
					m_auto_mat_action_cache[15] = m_auto_mat_action_cache[8] * m_auto_mat_action_cache[7];
				});
			}
			else if (m_auto_mat_do_calc[14]) {
				m_auto_mat_calcers.push_back([this]() {
					m_auto_mat_action_cache[15] = m_auto_mat_action_cache[14] * m_auto_mat_action_cache[1];
				});
			}
			else if (m_auto_mat_do_calc[3] && m_auto_mat_do_calc[12]) {
				m_auto_mat_calcers.push_back([this]() {
					m_auto_mat_action_cache[15] = m_auto_mat_action_cache[12] * m_auto_mat_action_cache[3];
				});
			}
			else if (m_auto_mat_do_calc[3]) {
				m_auto_mat_calcers.push_back([this]() {
					m_auto_mat_action_cache[15] = m_auto_mat_action_cache[8] * m_auto_mat_action_cache[4] * m_auto_mat_action_cache[3];
				});
			}
			else if (m_auto_mat_do_calc[12]) {
				m_auto_mat_calcers.push_back([this]() {
					m_auto_mat_action_cache[15] = m_auto_mat_action_cache[12] * m_auto_mat_action_cache[2] * m_auto_mat_action_cache[1];
				});
			}
			else if (m_auto_mat_do_calc[6]) {
				m_auto_mat_calcers.push_back([this]() {
					m_auto_mat_action_cache[15] = m_auto_mat_action_cache[8] * m_auto_mat_action_cache[6] * m_auto_mat_action_cache[1];
				});
			}
			else {
				m_auto_mat_calcers.push_back([this]() {
					m_auto_mat_action_cache[15] = m_auto_mat_action_cache[8] * m_auto_mat_action_cache[4] * m_auto_mat_action_cache[2] * m_auto_mat_action_cache[1];
				});
			}
		}
	}

	//std::vector<const GLchar*> Shader::GetAsCStrs(const std::vector<std::string>& string_array)
	//{
	//	std::vector<const GLchar*> c_strs;
	//	c_strs.reserve(string_array.size());
	//	for (const auto& s : string_array)
	//	{
	//		c_strs.push_back(s.c_str());
	//	}
	//	return c_strs;
	//}

	std::string Shader::ConcatSources(const std::vector<std::string>& sources)
	{
		std::stringstream ss;
		for (const auto& s : sources)
		{
			ss << s;
		}
		return ss.str();
	}


	void Shader::PrepareAutoMatActionConfigs()
	{
		for (const auto& tpl : m_auto_matrices)
		{
			const auto& name = std::get<0>(tpl);
			auto params = std::get<1>(tpl);
			auto matidx = static_cast<uint8_t>(params) & 0xF;

			// Handle optional/mandatory
			auto is_optional = (params & AutoMatrix::IsOptional) != AutoMatrix::Nothing;
			auto is_mandatory = (params & AutoMatrix::IsMandatory) != AutoMatrix::Nothing;

			GLuint loc;
			if (is_mandatory)
				loc = GetMandatoryUniformLocation(name);
			else if (is_optional)
				loc = GetOptionalUniformLocation(name);
			else
				loc = GetUniformLocation(name);

			// Check which transformations to apply:
			auto is_normal_matrix = (params & AutoMatrix::IsNormalMatrix) != AutoMatrix::Nothing;
			auto do_transpose = (params & AutoMatrix::DoTranspose) != AutoMatrix::Nothing;
			auto do_invert = (params & AutoMatrix::DoInvert) != AutoMatrix::Nothing;

			if (is_normal_matrix)
			{
				if (do_transpose)
				{
					if (do_invert)
					{
						m_auto_mats_action_config.push_back([this, loc, matidx]() {
							SetUniform(loc, glm::mat3(m_auto_mat_action_cache[matidx]));
						});
					}
					else
					{
						m_auto_mats_action_config.push_back([this, loc, matidx]() {
							SetUniform(loc, glm::mat3(glm::inverse(m_auto_mat_action_cache[matidx])));
						});
					}
				}
				else if (do_invert)
				{
					m_auto_mats_action_config.push_back([this, loc, matidx]() {
						SetUniform(loc, glm::mat3(glm::transpose(m_auto_mat_action_cache[matidx])));
					});
				}
				else
				{
					m_auto_mats_action_config.push_back([this, loc, matidx]() {
						SetUniform(loc, glm::mat3(glm::inverseTranspose(m_auto_mat_action_cache[matidx])));
					});
				}
			}
			else
			{
				if (do_transpose)
				{
					if (do_invert)
					{
						m_auto_mats_action_config.push_back([this, loc, matidx]() {
							SetUniform(loc, glm::inverseTranspose(m_auto_mat_action_cache[matidx]));
						});
					}
					else
					{
						m_auto_mats_action_config.push_back([this, loc, matidx]() {
							SetUniform(loc, glm::transpose(m_auto_mat_action_cache[matidx]));
						});
					}
				}
				else if (do_invert)
				{
					m_auto_mats_action_config.push_back([this, loc, matidx]() {
						SetUniform(loc, glm::inverse(m_auto_mat_action_cache[matidx]));
					});
				}
				else
				{
					m_auto_mats_action_config.push_back([this, loc, matidx]() {
						SetUniform(loc, m_auto_mat_action_cache[matidx]);
					});
				}
			}
		}
	}


	std::string Shader::kNewLine = "\n";

	Shader& Shader::AddVertexShaderSourceFromFile(std::string path, bool append_newline)
	{
		return AddVertexShaderSource(LoadFromFile(path), append_newline);
	}

	Shader& Shader::AddTesselationControlShaderSourceFromFile(std::string path, bool append_newline)
	{
		return AddTesselationControlShaderSource(LoadFromFile(path), append_newline);
	}

	Shader& Shader::AddTesselationEvaluationShaderSourceFromFile(std::string path, bool append_newline)
	{
		return AddTesselationEvaluationShaderSource(LoadFromFile(path), append_newline);
	}

	Shader& Shader::AddGeometryShaderSourceFromFile(std::string path, bool append_newline)
	{
		return AddGeometryShaderSource(LoadFromFile(path), append_newline);
	}

	Shader& Shader::AddFragmentShaderSourceFromFile(std::string path, std::vector<std::tuple<GLuint, const GLchar*>> outputs, bool append_newline)
	{
		return AddFragmentShaderSource(LoadFromFile(path), outputs, append_newline);
	}

	Shader& Shader::AddComputeShaderSourceFromFile(std::string path, bool append_newline)
	{
		return AddComputeShaderSource(LoadFromFile(path), append_newline);
	}

	Shader& Shader::SetTransformFeedbackVaryings(std::vector<const char*> varyings, GLenum buffer_mode)
	{
		m_transform_feedback_varyings = std::move(varyings);
		m_transform_feedback_buffer_mode = buffer_mode;
		return *this;
	}

	Shader& Shader::AddVertexShaderSource(std::string shader_source, bool append_newline)
	{
		m_vertex_shader_sources.push_back(std::move(append_newline ? shader_source + kNewLine : shader_source));
		return *this;
	}

	Shader& Shader::AddTesselationControlShaderSource(std::string shader_source, bool append_newline)
	{
		m_tess_control_shader_sources.push_back(std::move(append_newline ? shader_source + kNewLine : shader_source));
		return *this;
	}

	Shader& Shader::AddTesselationEvaluationShaderSource(std::string shader_source, bool append_newline)
	{
		m_tess_eval_shader_sources.push_back(std::move(append_newline ? shader_source + kNewLine : shader_source));
		return *this;
	}

	Shader& Shader::AddGeometryShaderSource(std::string shader_source, bool append_newline)
	{
		m_geometry_shader_sources.push_back(std::move(append_newline ? shader_source + kNewLine : shader_source));
		return *this;
	}

	Shader& Shader::AddFragmentShaderSource(std::string shader_source, bool append_newline)
	{
		m_fragment_shader_sources.push_back(std::move(append_newline ? shader_source + kNewLine : shader_source));
		return *this;
	}

	Shader& Shader::AddFragmentShaderSource(std::string shader_source, std::vector<std::tuple<GLuint, const GLchar*>> outputs, bool append_newline)
	{
		AddFragmentShaderSource(std::move(append_newline ? shader_source + kNewLine : shader_source));
		AddFragmentShaderOutput(std::move(outputs));
		return *this;
	}

	Shader& Shader::AddFragmentShaderOutput(std::vector<std::tuple<GLuint, const GLchar*>> outputs)
	{
		m_fragment_outputs.insert(std::end(m_fragment_outputs), std::begin(outputs), std::end(outputs));
		return *this;
	}

	Shader& Shader::AddComputeShaderSource(std::string shader_source, bool append_newline)
	{
		m_compute_shader_sources.push_back(std::move(append_newline ? shader_source + kNewLine : shader_source));
		return *this;
	}

	Shader& Shader::AddToMultipleShaderSources(std::string shader_source, ShaderType which_shaders, bool append_newline)
	{
		auto shader_src_with_or_without_newline = append_newline ? shader_source + kNewLine : shader_source;
		if (ShaderType::None != (which_shaders & ShaderType::Vertex))
		{
			AddVertexShaderSource(shader_src_with_or_without_newline);
		}
		if (ShaderType::None != (which_shaders & ShaderType::TessControl))
		{
			AddTesselationControlShaderSource(shader_src_with_or_without_newline);
		}
		if (ShaderType::None != (which_shaders & ShaderType::TessEval))
		{
			AddTesselationEvaluationShaderSource(shader_src_with_or_without_newline);
		}
		if (ShaderType::None != (which_shaders & ShaderType::Geometry))
		{
			AddGeometryShaderSource(shader_src_with_or_without_newline);
		}
		if (ShaderType::None != (which_shaders & ShaderType::Fragment))
		{
			AddFragmentShaderSource(shader_src_with_or_without_newline);
		}
		if (ShaderType::None != (which_shaders & ShaderType::Compute))
		{
			AddComputeShaderSource(shader_src_with_or_without_newline);
		}
		return *this;
	}

	bool Shader::has_tesselation_shaders() const
	{
		return 0 != m_shaderHandles[1] && 0 != m_shaderHandles[2];
	}

	GLint Shader::patch_vertices() const
	{
		return m_patch_vertices;
	}

	VertexAttribData Shader::vertex_attrib_config() const
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

	bool Shader::has_geometry_shader() const
	{
		return 0 != m_shaderHandles[3];
	}

	void Shader::DetermineTessData()
	{
		if (has_tesselation_shaders())
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

		auto attrib_config = VertexAttribData::Nothing;

		bool has_bone_weights = false;
		bool has_bone_indices = false;

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
					attrib_config = attrib_config | VertexAttribData::Position;
					log_debug_verbose("For shader with handle[%u], positions attribute is named [%s]", handle(), name);
					break;
				case VertexAttribLocation::TexCoord:
					attrib_config = attrib_config | VertexAttribData::Tex2D;
					log_debug_verbose("For shader with handle[%u], tex-coords attribute is named [%s]", handle(), name);
					break;
				case VertexAttribLocation::Normal:
					attrib_config = attrib_config | VertexAttribData::Normal;
					log_debug_verbose("For shader with handle[%u], normals attribute is named [%s]", handle(), name);
					break;
				case VertexAttribLocation::Color:
					attrib_config = attrib_config | VertexAttribData::Color;
					log_debug_verbose("For shader with handle[%u], colors attribute is named [%s]", handle(), name);
					break;
				case VertexAttribLocation::TexCoord2:
					attrib_config = attrib_config | VertexAttribData::Tex2D_2;
					log_debug_verbose("For shader with handle[%u], tex-coords2 attribute is named [%s]", handle(), name);
					break;
				case VertexAttribLocation::TexCoord3:
					attrib_config = attrib_config | VertexAttribData::Tex2D_3;
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
					attrib_config = attrib_config | VertexAttribData::Tangents;
					log_debug_verbose("For shader with handle[%u], tangents attribute is named [%s]", handle(), name);
					break;
				case VertexAttribLocation::Bitangents:
					attrib_config = attrib_config | VertexAttribData::Bitangents;
					log_debug_verbose("For shader with handle[%u], bitangents attribute is named [%s]", handle(), name);
					break;
				case VertexAttribLocation::Param0:
					attrib_config = attrib_config | VertexAttribData::Param0;
					log_debug_verbose("For shader with handle[%u], param0 attribute is named [%s]", handle(), name);
					break;
				case VertexAttribLocation::Param1:
					attrib_config = attrib_config | VertexAttribData::Param1;
					log_debug_verbose("For shader with handle[%u], param1 attribute is named [%s]", handle(), name);
					break;
				case VertexAttribLocation::Param2:
					attrib_config = attrib_config | VertexAttribData::Param2;
					log_debug_verbose("For shader with handle[%u], param2 attribute is named [%s]", handle(), name);
					break;
				case VertexAttribLocation::Param3:
					attrib_config = attrib_config | VertexAttribData::Param3;
					log_debug_verbose("For shader with handle[%u], param3 attribute is named [%s]", handle(), name);
					break;
				case VertexAttribLocation::Param4:
					attrib_config = attrib_config | VertexAttribData::Param4;
					log_debug_verbose("For shader with handle[%u], param4 attribute is named [%s]", handle(), name);
					break;
				case VertexAttribLocation::Param5:
					attrib_config = attrib_config | VertexAttribData::Param5;
					log_debug_verbose("For shader with handle[%u], param5 attribute is named [%s]", handle(), name);
					break;
				}
			}
		}

		if (has_bone_weights && has_bone_indices)
		{
			attrib_config = attrib_config | VertexAttribData::Bones;
		}

		m_vertex_attrib_config = attrib_config;
	}

	void Shader::DeterminePrimitivesMode()
	{
		if (has_tesselation_shaders())
		{
			m_kind_of_primitives = GL_PATCHES;
		}
		else
		{
			m_kind_of_primitives = GL_TRIANGLES;
		}
	}

	void Shader::SetAutoMatrices(const glm::mat4& transformationMatrix, const glm::mat4& modelMatrix, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix)
	{
		// update the four base matrices in cache
		m_auto_mat_action_cache[static_cast<uint8_t>(AutoMatrix::TransformMatrix)] = transformationMatrix;
		m_auto_mat_action_cache[static_cast<uint8_t>(AutoMatrix::ModelMatrix)] = modelMatrix;
		m_auto_mat_action_cache[static_cast<uint8_t>(AutoMatrix::ViewMatrix)] = viewMatrix;
		m_auto_mat_action_cache[static_cast<uint8_t>(AutoMatrix::ProjectionMatrix)] = projectionMatrix;
		// update the rest of the matrices
		for (const auto& fu : m_auto_mat_calcers)
		{
			fu();
		}
		// upload matrices to shader
		for (const auto& fu : m_auto_mats_action_config)
		{
			fu();
		}
	}

	Shader& Shader::Build()
	{
		m_shaderHandles[0] = m_vertex_shader_sources.empty()		? 0 : Compile(ConcatSources(m_vertex_shader_sources),		GL_VERTEX_SHADER);
		m_shaderHandles[1] = m_tess_control_shader_sources.empty()	? 0 : Compile(ConcatSources(m_tess_control_shader_sources),	GL_TESS_CONTROL_SHADER);
		m_shaderHandles[2] = m_tess_eval_shader_sources.empty()		? 0 : Compile(ConcatSources(m_tess_eval_shader_sources),	GL_TESS_EVALUATION_SHADER);
		m_shaderHandles[3] = m_geometry_shader_sources.empty()		? 0 : Compile(ConcatSources(m_geometry_shader_sources),		GL_GEOMETRY_SHADER);
		m_shaderHandles[4] = m_fragment_shader_sources.empty()		? 0	: Compile(ConcatSources(m_fragment_shader_sources),		GL_FRAGMENT_SHADER);
		m_shaderHandles[5] = m_compute_shader_sources.empty()		? 0	: Compile(ConcatSources(m_compute_shader_sources),      GL_COMPUTE_SHADER);

		const auto progHandle = glCreateProgram();
		m_prog_handle = progHandle;

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

		if (m_transform_feedback_varyings.size() > 0)
		{
			glTransformFeedbackVaryings(progHandle, static_cast<GLsizei>(m_transform_feedback_varyings.size()), &m_transform_feedback_varyings[0], m_transform_feedback_buffer_mode);
			CheckErrorAndPrintInfoLog("Shader::Build after glTransformFeedbackVaryings", "Could not set Transform Feedback Varyings");
		}

		glLinkProgram(progHandle);
		GLint linkSuccess;
		glGetProgramiv(progHandle, GL_LINK_STATUS, &linkSuccess);

		if (linkSuccess == GL_FALSE)
		{
			PrintInfoLog("Linking shader program failed");
			Destroy();
			throw ExceptionWithCallstack("Linking shader program failed");
		}

		// success!
		DetermineVertexAttribConfig();
		DetermineTessData();
		DeterminePrimitivesMode();

		DetermineWhichAutoMatsToCalc();
		PrepareAutoMatActionConfigs();
		CreateAutoMatCalcers();

#if defined(_DEBUG) && defined(FEATURE_NOT_READY_YET)
		m_files_changed = [this]()
		{
			log_info("RELOAD THIS SHADER!");
			Shader nuShader;
			nuShader.AddToMultipleShaderSources(Shader::version_string(), ShaderType::Vertex | ShaderType::Fragment)
				.AddVertexShaderSourceFromFile("assets/shaders/blinnphong.vert")
				.AddFragmentShaderSourceFromFile("assets/shaders/blinnphong.frag", { std::make_tuple(0, "oFragColor") })
				.Build();
			*this = std::move(nuShader);
		};
		std::vector<std::string> files;
		files.push_back("assets/shaders/blinnphong.vert");
		files.push_back("assets/shaders/blinnphong.frag");
		Engine::current()->NotifyOnFileChanges(std::move(files), &m_files_changed);
#endif

		CheckErrorAndPrintInfoLog("Shader::Build END", "Something went wrong");
		return *this;
	}

	Shader& Shader::QueryOptionalUniformLocations(const std::vector<std::string>& names)
	{
		if (0 == m_prog_handle)
		{
			throw ExceptionWithCallstack("QueryUniformLocations is useless since the program handle is 0");
		}

		glUseProgram(m_prog_handle);
		for (auto const& name : names)
		{
			const auto loc = glGetUniformLocation(m_prog_handle, name.c_str());
			m_uniform_locations[name] = loc;
		}
		return *this;
	}

	Shader& Shader::QueryUniformLocations(const std::vector<std::string>& names)
	{
		for (auto const& name : names)
		{
			if (-1 == m_uniform_locations[name])
			{
				log_warning("Uniform location of '%s' not found.", name.c_str());
			}
		}
		return *this;
	}
		
	Shader& Shader::QueryMandatoryUniformLocations(const std::vector<std::string>& names)
	{
		for (auto const& name : names)
		{
			if (-1 == m_uniform_locations[name])
			{
				throw ExceptionWithCallstack("Uniform location of '" + name + "' not found.");
			}
		}
		return *this;
	}

	Shader& Shader::QueryOptionalUniformLocation(const std::string& name)
	{
		if (0 == m_prog_handle)
		{
			throw ExceptionWithCallstack("QueryUniformLocations is useless since the program handle is 0");
		}

		glUseProgram(m_prog_handle);
		auto loc = glGetUniformLocation(m_prog_handle, name.c_str());
		m_uniform_locations[name] = loc;
		return *this;
	}

	Shader& Shader::QueryUniformLocation(const std::string& name)
	{
		QueryOptionalUniformLocation(name);
		if (-1 == m_uniform_locations[name])
		{
			log_warning("Uniform location of '%s' not found.", name.c_str());
		}
		return *this;
	}

	Shader& Shader::QueryMandatoryUniformLocation(const std::string& name)
	{
		QueryOptionalUniformLocation(name);
		if (-1 == m_uniform_locations[name])
		{
			throw ExceptionWithCallstack("Uniform location of '" + name + "' not found.");
		}
		return *this;
	}

	Shader& Shader::QueryOptionalUniformBlockIndex(const std::string& name)
	{
		if (0 == m_prog_handle)
		{
			throw ExceptionWithCallstack("QueryUniformBlockIndex is useless since the program handle is 0");
		}

		glUseProgram(m_prog_handle);
		auto loc = glGetUniformBlockIndex(m_prog_handle, name.c_str());
		m_uniform_block_indices[name] = loc;
		return *this;
	}
	
	Shader& Shader::QueryUniformBlockIndex(const std::string& name)
	{
		QueryOptionalUniformBlockIndex(name);
		if (-1 == m_uniform_block_indices[name])
		{
			log_warning("Uniform block index location of '%s' not found.", name.c_str());
		}
		return *this;
	}
	
	Shader& Shader::QueryMandatoryUniformBlockIndex(const std::string& name)
	{
		QueryOptionalUniformBlockIndex(name);
		if (-1 == m_uniform_block_indices[name])
		{
			throw ExceptionWithCallstack("Uniform block index location of '" + name + "' not found.");
		}
		return *this;
	}

	Shader& Shader::DeclareAutoMatrix(std::string name, AutoMatrix properties)
	{
		for (const auto& tpl : m_auto_matrices)
		{
			if (std::get<0>(tpl) == name)
				throw ExceptionWithCallstack("AutoMatrix declaration for '" + name + "' already exists.");
		}
		m_auto_matrices.push_back(std::make_tuple(std::move(name), properties));
		return *this;
	}

	Shader& Shader::Destroy()
	{
		if (0 == m_prog_handle)
		{
			return *this;
		}

		GLsizei count = 0;
		GLuint attachedShaders[8]; // fragment, vertex, geometry, hull, domai || compute => max. 5 aligned to 8
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

	GLuint Shader::GetOptionalUniformLocation(const std::string& name)
	{
		const auto loc = m_uniform_locations.find(name);
		if (loc == m_uniform_locations.end())
		{
			QueryOptionalUniformLocation(name);
			return m_uniform_locations.at(name);
		}
		return loc->second;
	}

	GLuint Shader::GetUniformLocation(const std::string& name)
	{
		const auto loc = m_uniform_locations.find(name);
		if (loc == m_uniform_locations.end())
		{
			QueryUniformLocation(name);
			return m_uniform_locations.at(name);
		}
		return loc->second;
	}

	GLuint Shader::GetMandatoryUniformLocation(const std::string& name)
	{
		const auto loc = m_uniform_locations.find(name);
		if (loc == m_uniform_locations.end())
		{
			QueryMandatoryUniformLocation(name);
			return m_uniform_locations.at(name);
		}
		return loc->second;
	}

	GLuint Shader::GetOptionalUniformBlockIndex(const std::string& name)
	{
		const auto loc = m_uniform_block_indices.find(name);
		if (loc == m_uniform_block_indices.end())
		{
			QueryOptionalUniformBlockIndex(name);
			return m_uniform_block_indices.at(name);
		}
		return loc->second;
	}

	GLuint Shader::GetUniformBlockIndex(const std::string& name)
	{
		const auto loc = m_uniform_block_indices.find(name);
		if (loc == m_uniform_block_indices.end())
		{
			QueryUniformBlockIndex(name);
			return m_uniform_block_indices.at(name);
		}
		return loc->second;
	}

	GLuint Shader::GetMandatoryUniformBlockIndex(const std::string& name)
	{
		const auto loc = m_uniform_block_indices.find(name);
		if (loc == m_uniform_block_indices.end())
		{
			QueryMandatoryUniformBlockIndex(name);
			return m_uniform_block_indices.at(name);
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

	std::string Shader::version_string()
	{
		auto version = std::string("#version ") + std::to_string(Context::current()->gl_major_version() * 100 + Context::current()->gl_minor_version() * 10);
		return Context::current()->is_core_profile() ? version + " core" : version;
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

	GLuint Shader::Compile(const std::string& complete_source, GLenum shaderType)
	{
		auto shaderHandle = glCreateShader(shaderType);
		if (!shaderHandle)
		{
			return GLuint(0);
		}

		const auto* source_c = static_cast<const GLchar*>(complete_source.c_str());
		glShaderSource(shaderHandle, 1, &source_c, 0);
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

	void RenderMeshes(const Shader& shader, const MeshVaosForAttribConfig& meshes_and_their_vaos)
	{
		// If the following assert fails, you are probably trying to render using the wrong shader!
		assert(shader.vertex_attrib_config() == meshes_and_their_vaos.m_vertex_attrib_config);

		GLenum mode = shader.kind_of_primitives();
		if (GL_PATCHES == mode)
		{
			glPatchParameteri(GL_PATCH_VERTICES, shader.patch_vertices());
		}

		for (auto& tupl : meshes_and_their_vaos.m_mesh_vaos)
		{
			Mesh& mesh = std::get<0>(tupl);
			VAOType vao = std::get<1>(tupl);

			glBindVertexArray(vao);
			glDrawElements(mode, mesh.indices_length(), GL_UNSIGNED_INT, nullptr);
		}
	}

	void RenderMeshesWithAlignedUniformSetters(const Shader& shader, const MeshVaosForAttribConfig& meshes_and_their_vaos, const MeshUniformSettersForShader& uniform_setters)
	{
		// If one of the following asserts fails, you are probably trying to render using the wrong shader!
		assert(shader.vertex_attrib_config() == meshes_and_their_vaos.m_vertex_attrib_config);
		//assert(shader.handle() == uniform_setters.m_shader_handle);

		GLenum mode = shader.kind_of_primitives();
		if (GL_PATCHES == mode)
		{
			glPatchParameteri(GL_PATCH_VERTICES, shader.patch_vertices());
		}

		assert(meshes_and_their_vaos.m_mesh_vaos.size() == uniform_setters.m_mesh_uniform_setters.size());
		auto n = meshes_and_their_vaos.m_mesh_vaos.size();
		for (auto i = 0; i < n; ++i)
		{
			auto& tupl = meshes_and_their_vaos.m_mesh_vaos[i];
			Mesh& mesh = std::get<0>(tupl);
			assert(&mesh == &static_cast<Mesh&>(std::get<0>(uniform_setters.m_mesh_uniform_setters[i])));
			VAOType vao = std::get<1>(tupl);
			std::get<1>(uniform_setters.m_mesh_uniform_setters[i])(shader, *mesh.material_data());

			glBindVertexArray(vao);
			glDrawElements(mode, mesh.indices_length(), GL_UNSIGNED_INT, nullptr);
		}
	}

	void UnbindVAO()
	{
		glBindVertexArray(0);
	}
}
