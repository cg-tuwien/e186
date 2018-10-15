#include "MaterialData.h"
#include "MaterialUniformLocation.h"

namespace e186
{
	MaterialData::MaterialData() :
		m_name(""),
		m_hidden(false),
		m_diffuse_reflectivity(1.0f, 1.0f, 1.0f),
		m_specular_reflectivity(1.0f, 1.0f, 1.0f),
		m_ambient_reflectivity(1.0f, 1.0f, 1.0f),
		m_emissive_color(1.0f, 1.0f, 1.0f),
		m_transparent_color(1.0f, 1.0f, 1.0f),
		m_wireframe_mode(false),
		m_twosided(false),
		m_blend_mode(BlendMode::AlphaBlended),
		m_opacity(1.0f),
		m_shininess(0.0f),
		m_shininess_strength(0.0f),
		m_refraction_index(0.0f),
		m_reflectivity(0.0f),
		m_albedo(0.0f, 0.0f, 0.0f),
		m_metallic(0.0f),
		m_smoothness(0.0f),
		m_roughness(0.0f),
		m_tiling(1.0f, 1.0f),
		m_offset(0.0f, 0.0f)
	{ 
	}

	TexParams MaterialData::ai_mapping_mode_to_tex_params(aiTextureMapMode aimm)
	{
		switch (aimm)
		{
		case aiTextureMapMode_Wrap:
			return TexParams::Repeat;
		case aiTextureMapMode_Clamp:
			return TexParams::ClampToEdge;
		case aiTextureMapMode_Decal:
			return TexParams::ClampToEdge;
		case aiTextureMapMode_Mirror:
			return TexParams::MirroredRepeat;
		default:
			return TexParams::Repeat;
		}
	}

	const std::shared_ptr<Tex2D>& MaterialData::GetOrCreateWhiteTex()
	{
		static auto white_tex = []() 
		{ 
			auto tex = std::make_shared<Tex2D>();
			tex->Generate1pxTexture(255, 255, 255).Upload().BindAndSetTextureParameters(TexParams::NearestFiltering | TexParams::ClampToEdge);
			return tex;
		}();
		return white_tex;
	}

	const std::shared_ptr<Tex2D>& MaterialData::GetOrCreateBlackTex()
	{
		static auto black_tex = []()
		{
			auto tex = std::make_shared<Tex2D>();
			tex->Generate1pxTexture(0, 0, 0).Upload().BindAndSetTextureParameters(TexParams::NearestFiltering | TexParams::ClampToEdge);
			return tex;
		}();
		return black_tex;
	}

	const std::shared_ptr<Tex2D>& MaterialData::GetOrCreateStraightUpNormalTex()
	{
		static auto straight_up_normal_tex = []()
		{
			auto tex = std::make_shared<Tex2D>();
			tex->Generate1pxTexture(127, 127, 255).Upload().BindAndSetTextureParameters(TexParams::NearestFiltering | TexParams::ClampToEdge);
			return tex;
		}();
		return straight_up_normal_tex;
	}

	MaterialData::MaterialData(aiMaterial* aimat, TexLoadingHelper& tlh) : MaterialData()
	{
		aiString strVal;
		aiColor3D color(0.0f, 0.0f, 0.0f);
		int intVal;
		aiBlendMode blendMode;
		float floatVal;
		aiTextureMapping texMapping;
		aiTextureMapMode texMapMode;
		
		if (AI_SUCCESS == aimat->Get(AI_MATKEY_NAME, strVal)) {
			m_name = strVal.data;
		}

		if (AI_SUCCESS == aimat->Get(AI_MATKEY_COLOR_DIFFUSE, color)) {
			m_diffuse_reflectivity = glm::vec3(color.r, color.g, color.b);
		}

		if (AI_SUCCESS == aimat->Get(AI_MATKEY_COLOR_SPECULAR, color)) {
			m_specular_reflectivity = glm::vec3(color.r, color.g, color.b);
		}

		if (AI_SUCCESS == aimat->Get(AI_MATKEY_COLOR_AMBIENT, color)) {
			m_ambient_reflectivity = glm::vec3(color.r, color.g, color.b);
		}

		if (AI_SUCCESS == aimat->Get(AI_MATKEY_COLOR_EMISSIVE, color)) {
			m_emissive_color = glm::vec3(color.r, color.g, color.b);
		}

		if (AI_SUCCESS == aimat->Get(AI_MATKEY_ENABLE_WIREFRAME, intVal)) {
			m_wireframe_mode = 0 != intVal;
		}

		if (AI_SUCCESS == aimat->Get(AI_MATKEY_TWOSIDED, intVal)) {
			m_twosided = 0 != intVal;
		}

		if (AI_SUCCESS == aimat->Get(AI_MATKEY_BLEND_FUNC, blendMode)) {
			m_blend_mode = blendMode == aiBlendMode_Additive
				? BlendMode::Additive
				: BlendMode::AlphaBlended;
		}

		if (AI_SUCCESS == aimat->Get(AI_MATKEY_OPACITY, floatVal)) {
			m_opacity = floatVal;
		}

		if (AI_SUCCESS == aimat->Get(AI_MATKEY_SHININESS, floatVal)) {
			m_shininess = floatVal;
		}

		if (AI_SUCCESS == aimat->Get(AI_MATKEY_SHININESS_STRENGTH, floatVal)) {
			m_shininess_strength = floatVal;
		}

		if (AI_SUCCESS == aimat->Get(AI_MATKEY_REFRACTI, floatVal)) {
			m_refraction_index = floatVal;
		}

		if (AI_SUCCESS == aimat->Get(AI_MATKEY_REFLECTIVITY, floatVal)) {
			m_reflectivity = floatVal;
		}

		if (AI_SUCCESS == aimat->GetTexture(aiTextureType_DIFFUSE, 0, &strVal, &texMapping, nullptr, nullptr, nullptr, &texMapMode)) {
			if (texMapping != aiTextureMapping_UV) {
				log_warning("Unsupported texture mapping mode[%u] for texture[%s]", static_cast<unsigned int>(texMapping), strVal.C_Str());
			}
			auto tex = tlh.GetOrLoadTex(strVal.data, tlh.general_tex_params() | ai_mapping_mode_to_tex_params(texMapMode) | TexParams::GenerateMipMaps, true);
			m_diffuse_tex = tex;
		}

		if (AI_SUCCESS == aimat->GetTexture(aiTextureType_SPECULAR, 0, &strVal, &texMapping, nullptr, nullptr, nullptr, &texMapMode)) {
			if (texMapping != aiTextureMapping_UV) {
				log_warning("Unsupported texture mapping mode[%u] for texture[%s]", static_cast<unsigned int>(texMapping), strVal.C_Str());
			}
			auto tex = tlh.GetOrLoadTex(strVal.data, tlh.general_tex_params() | ai_mapping_mode_to_tex_params(texMapMode) | TexParams::GenerateMipMaps);
			m_specular_tex = tex;
		}

		if (AI_SUCCESS == aimat->GetTexture(aiTextureType_AMBIENT, 0, &strVal, &texMapping, nullptr, nullptr, nullptr, &texMapMode)) {
			if (texMapping != aiTextureMapping_UV) {
				log_warning("Unsupported texture mapping mode[%u] for texture[%s]", static_cast<unsigned int>(texMapping), strVal.C_Str());
			}
			auto tex = tlh.GetOrLoadTex(strVal.data, tlh.general_tex_params() | ai_mapping_mode_to_tex_params(texMapMode) | TexParams::GenerateMipMaps, true);
			m_ambient_tex = tex;
		}

		if (AI_SUCCESS == aimat->GetTexture(aiTextureType_EMISSIVE, 0, &strVal, &texMapping, nullptr, nullptr, nullptr, &texMapMode)) {
			if (texMapping != aiTextureMapping_UV) {
				log_warning("Unsupported texture mapping mode[%u] for texture[%s]", static_cast<unsigned int>(texMapping), strVal.C_Str());
			}
			auto tex = tlh.GetOrLoadTex(strVal.data, tlh.general_tex_params() | ai_mapping_mode_to_tex_params(texMapMode) | TexParams::GenerateMipMaps);
			m_emissive_tex = tex;
		}

		if (AI_SUCCESS == aimat->GetTexture(aiTextureType_HEIGHT, 0, &strVal, &texMapping, nullptr, nullptr, nullptr, &texMapMode)) {
			if (texMapping != aiTextureMapping_UV) {
				log_warning("Unsupported texture mapping mode[%u] for texture[%s]", static_cast<unsigned int>(texMapping), strVal.C_Str());
			}
			auto tex = tlh.GetOrLoadTex(strVal.data, tlh.general_tex_params() | ai_mapping_mode_to_tex_params(texMapMode) | TexParams::GenerateMipMaps);
			m_height_tex = tex;
		}

		if (AI_SUCCESS == aimat->GetTexture(aiTextureType_NORMALS, 0, &strVal, &texMapping, nullptr, nullptr, nullptr, &texMapMode)) {
			if (texMapping != aiTextureMapping_UV) {
				log_warning("Unsupported texture mapping mode[%u] for texture[%s]", static_cast<unsigned int>(texMapping), strVal.C_Str());
			}
			auto tex = tlh.GetOrLoadTex(strVal.data, tlh.general_tex_params() | ai_mapping_mode_to_tex_params(texMapMode) | TexParams::GenerateMipMaps);
			m_normals_tex = tex;
		}

		if (AI_SUCCESS == aimat->GetTexture(aiTextureType_SHININESS, 0, &strVal, &texMapping, nullptr, nullptr, nullptr, &texMapMode)) {
			if (texMapping != aiTextureMapping_UV) {
				log_warning("Unsupported texture mapping mode[%u] for texture[%s]", static_cast<unsigned int>(texMapping), strVal.C_Str());
			}
			auto tex = tlh.GetOrLoadTex(strVal.data, tlh.general_tex_params() | ai_mapping_mode_to_tex_params(texMapMode) | TexParams::GenerateMipMaps);
			m_shininess_tex = tex;
		}

		if (AI_SUCCESS == aimat->GetTexture(aiTextureType_OPACITY, 0, &strVal, &texMapping, nullptr, nullptr, nullptr, &texMapMode)) {
			if (texMapping != aiTextureMapping_UV) {
				log_warning("Unsupported texture mapping mode[%u] for texture[%s]", static_cast<unsigned int>(texMapping), strVal.C_Str());
			}
			auto tex = tlh.GetOrLoadTex(strVal.data, tlh.general_tex_params() | ai_mapping_mode_to_tex_params(texMapMode) | TexParams::GenerateMipMaps);
			m_opacity_tex = tex;
		}

		if (AI_SUCCESS == aimat->GetTexture(aiTextureType_DISPLACEMENT, 0, &strVal, &texMapping, nullptr, nullptr, nullptr, &texMapMode)) {
			if (texMapping != aiTextureMapping_UV) {
				log_warning("Unsupported texture mapping mode[%u] for texture[%s]", static_cast<unsigned int>(texMapping), strVal.C_Str());
			}
			auto tex = tlh.GetOrLoadTex(strVal.data, tlh.general_tex_params() | ai_mapping_mode_to_tex_params(texMapMode));
			m_displacement_tex = tex;
		}

		if (AI_SUCCESS == aimat->GetTexture(aiTextureType_REFLECTION, 0, &strVal, &texMapping, nullptr, nullptr, nullptr, &texMapMode)) {
			if (texMapping != aiTextureMapping_UV) {
				log_warning("Unsupported texture mapping mode[%u] for texture[%s]", static_cast<unsigned int>(texMapping), strVal.C_Str());
			}
			auto tex = tlh.GetOrLoadTex(strVal.data, tlh.general_tex_params() | ai_mapping_mode_to_tex_params(texMapMode) | TexParams::GenerateMipMaps);
			m_reflection_tex = tex;
		}

		if (AI_SUCCESS == aimat->GetTexture(aiTextureType_LIGHTMAP, 0, &strVal, &texMapping, nullptr, nullptr, nullptr, &texMapMode)) {
			if (texMapping != aiTextureMapping_UV) {
				log_warning("Unsupported texture mapping mode[%u] for texture[%s]", static_cast<unsigned int>(texMapping), strVal.C_Str());
			}
			auto tex = tlh.GetOrLoadTex(strVal.data, tlh.general_tex_params() | ai_mapping_mode_to_tex_params(texMapMode) | TexParams::GenerateMipMaps);
			m_lightmap_tex = tex;
		}
	}

	UniformSetter CreateUniformSetterForShader(Shader& shader, MaterialData& material_data_info)
	{
		return CreateUniformSetterForShader(shader, UniformSetterUsageMode::Static, &material_data_info);
	}

	UniformSetter CreateUniformSetterForShader(Shader& shader, UniformSetterUsageMode usage_mode, MaterialData* material_data_info)
	{
		shader.Use();

		std::vector<UniformSubSetter> setter_funcs;

		GLchar name[GL_ACTIVE_UNIFORM_MAX_LENGTH];
		*name = 0;
		GLsizei name_len = 0;
		GLint size_of_the_uniform_variable = 0;
		GLenum type_of_the_uniform_variable = 0;

		int32_t current_tex_unit = 2;

		GLint count;
		glGetProgramiv(shader.handle(), GL_ACTIVE_UNIFORMS, &count);
		for (int i = 0; i < count; ++i)
		{
			name_len = 0;
			glGetActiveUniform(shader.handle(), i, GL_ACTIVE_UNIFORM_MAX_LENGTH, &name_len, &size_of_the_uniform_variable, &type_of_the_uniform_variable, name);
			auto location = glGetUniformLocation(shader.handle(), name);
			if (is_valid_MaterialUniformLocation(location))
			{
				switch (static_cast<MaterialUniformLocation>(location))
				{
					case MaterialUniformLocation::DiffuseColor:
						log_debug_verbose("In MaterialData[0x%p] for shader with handle[%u], diffuse reflectivity uniform is named [%s]", material_data_info, shader.handle(), name);
						setter_funcs.emplace_back( [](const Shader& shader, const MaterialData& mat, UniformSubSetter&)
						{
							shader.SetUniform(static_cast<GLuint>(MaterialUniformLocation::DiffuseColor), mat.diffuse_reflectivity());
						} );
						break;
					case MaterialUniformLocation::SpecularColor:
						log_debug_verbose("In MaterialData[0x%p] for shader with handle[%u], specular reflectivity uniform is named [%s]", material_data_info, shader.handle(), name);
						setter_funcs.emplace_back( [](const Shader& shader, const MaterialData& mat, UniformSubSetter&)
						{
							shader.SetUniform(static_cast<GLuint>(MaterialUniformLocation::SpecularColor), mat.specular_reflectivity());
						} );
						break;
					case MaterialUniformLocation::AmbientColor:
						log_debug_verbose("In MaterialData[0x%p] for shader with handle[%u], ambient reflectivity uniform is named [%s]", material_data_info, shader.handle(), name);
						setter_funcs.emplace_back( [](const Shader& shader, const MaterialData& mat, UniformSubSetter&)
						{
							shader.SetUniform(static_cast<GLuint>(MaterialUniformLocation::AmbientColor), mat.ambient_reflectivity());
						} );
						break;
					case MaterialUniformLocation::EmissiveColor:
						log_debug_verbose("In MaterialData[0x%p] for shader with handle[%u], emissive color uniform is named [%s]", material_data_info, shader.handle(), name);
						setter_funcs.emplace_back( [](const Shader& shader, const MaterialData& mat, UniformSubSetter&)
						{
							shader.SetUniform(static_cast<GLuint>(MaterialUniformLocation::EmissiveColor), mat.emissive_color());
						} );
						break;
					case MaterialUniformLocation::TransparentColor:
						log_debug_verbose("In MaterialData[0x%p] for shader with handle[%u], transparent color uniform is named [%s]", material_data_info, shader.handle(), name);
						setter_funcs.emplace_back( [](const Shader& shader, const MaterialData& mat, UniformSubSetter&)
						{
							shader.SetUniform(static_cast<GLuint>(MaterialUniformLocation::TransparentColor), mat.transparent_color());
						} );
						break;
					case MaterialUniformLocation::Opacity:
						log_debug_verbose("In MaterialData[0x%p] for shader with handle[%u], opacity uniform is named [%s]", material_data_info, shader.handle(), name);
						setter_funcs.emplace_back( [](const Shader& shader, const MaterialData& mat, UniformSubSetter&)
						{
							shader.SetUniform(static_cast<GLuint>(MaterialUniformLocation::Opacity), mat.opacity());
						} );
						break;
					case MaterialUniformLocation::Shininess:
						log_debug_verbose("In MaterialData[0x%p] for shader with handle[%u], shininess uniform is named [%s]", material_data_info, shader.handle(), name);
						setter_funcs.emplace_back( [](const Shader& shader, const MaterialData& mat, UniformSubSetter&)
						{
							shader.SetUniform(static_cast<GLuint>(MaterialUniformLocation::Shininess), mat.shininess());
						} );
						break;
					case MaterialUniformLocation::ShininessStrength:
						log_debug_verbose("In MaterialData[0x%p] for shader with handle[%u], shininess strength uniform is named [%s]", material_data_info, shader.handle(), name);
						setter_funcs.emplace_back( [](const Shader& shader, const MaterialData& mat, UniformSubSetter&)
						{
							shader.SetUniform(static_cast<GLuint>(MaterialUniformLocation::ShininessStrength), mat.shininess_strength());
						} );
						break;
					case MaterialUniformLocation::RefractionIndex:
						log_debug_verbose("In MaterialData[0x%p] for shader with handle[%u], refraction index uniform is named [%s]", material_data_info, shader.handle(), name);
						setter_funcs.emplace_back( [](const Shader& shader, const MaterialData& mat, UniformSubSetter&)
						{
							shader.SetUniform(static_cast<GLuint>(MaterialUniformLocation::RefractionIndex), mat.refraction_index());
						} );
						break;
					case MaterialUniformLocation::Reflectivity:
						log_debug_verbose("In MaterialData[0x%p] for shader with handle[%u], reflectivity uniform is named [%s]", material_data_info, shader.handle(), name);
						setter_funcs.emplace_back( [](const Shader& shader, const MaterialData& mat, UniformSubSetter&)
						{
							shader.SetUniform(static_cast<GLuint>(MaterialUniformLocation::Reflectivity), mat.reflectivity());
						} );
						break;
					case MaterialUniformLocation::DiffuseTexture:
						log_debug_verbose("In MaterialData[0x%p] for shader with handle[%u], diffuse texture sampler is named [%s]", material_data_info, shader.handle(), name);
						if (UniformSetterUsageMode::Dynamic == usage_mode)
						{
							setter_funcs.emplace_back( [tex_unit = current_tex_unit](const Shader& shader, const MaterialData& mat, UniformSubSetter&)
							{
								if (mat.diffuse_tex()) {
									shader.SetSampler(static_cast<GLuint>(MaterialUniformLocation::DiffuseTexture), *mat.diffuse_tex(), tex_unit);
								} else {
									shader.SetSampler(static_cast<GLuint>(MaterialUniformLocation::DiffuseTexture), *MaterialData::GetOrCreateWhiteTex(), tex_unit);
								}
							} );
						}
						else // Static usage
						{
							assert(UniformSetterUsageMode::Static == usage_mode);
							setter_funcs.emplace_back( [tex_unit = current_tex_unit](const Shader& shader, const MaterialData& mat, UniformSubSetter& setter)
							{
								// static usage mode => texture never changes => replace the UniformSetter's function!
								if (mat.diffuse_tex()) {
									setter.set_func([tex_unit](const Shader& shader, const MaterialData& mat, UniformSubSetter& setter) {
										shader.SetSampler(static_cast<GLuint>(MaterialUniformLocation::DiffuseTexture), *mat.diffuse_tex(), tex_unit);
									});
								}
								else {
									setter.set_func([tex_unit, white_tex = MaterialData::GetOrCreateWhiteTex()](const Shader& shader, const MaterialData& mat, UniformSubSetter& setter) {
										shader.SetSampler(static_cast<GLuint>(MaterialUniformLocation::DiffuseTexture), *white_tex, tex_unit);
									});
								}
							} );
						}
						current_tex_unit += 1;
						break;
					case MaterialUniformLocation::SpecularTexture:
						log_debug_verbose("In MaterialData[0x%p] for shader with handle[%u], specular texture sampler is named [%s]", material_data_info, shader.handle(), name);
						if (UniformSetterUsageMode::Dynamic == usage_mode)
						{
							setter_funcs.emplace_back( [tex_unit = current_tex_unit](const Shader& shader, const MaterialData& mat, UniformSubSetter&)
							{
								if (mat.specular_tex()) {
									shader.SetSampler(static_cast<GLuint>(MaterialUniformLocation::SpecularTexture), *mat.specular_tex(), tex_unit);
								}
								else {
									shader.SetSampler(static_cast<GLuint>(MaterialUniformLocation::SpecularTexture), *MaterialData::GetOrCreateWhiteTex(), tex_unit);
								}
							} );
						}
						else // Static usage
						{
							assert(UniformSetterUsageMode::Static == usage_mode);
							setter_funcs.emplace_back( [tex_unit = current_tex_unit](const Shader& shader, const MaterialData& mat, UniformSubSetter& setter)
							{
								// static usage mode => texture never changes => replace the UniformSetter's function!
								if (mat.specular_tex()) {
									setter.set_func([tex_unit](const Shader& shader, const MaterialData& mat, UniformSubSetter& setter) {
										shader.SetSampler(static_cast<GLuint>(MaterialUniformLocation::SpecularTexture), *mat.specular_tex(), tex_unit);
									});
								}
								else {
									setter.set_func([tex_unit, white_tex = MaterialData::GetOrCreateWhiteTex()](const Shader& shader, const MaterialData& mat, UniformSubSetter& setter) {
										shader.SetSampler(static_cast<GLuint>(MaterialUniformLocation::SpecularTexture), *white_tex, tex_unit);
									});
								}
							} );
						}
						current_tex_unit += 1;
						break;
					case MaterialUniformLocation::AmbientTexture:
						log_debug_verbose("In MaterialData[0x%p] for shader with handle[%u], ambient texture sampler is named [%s]", material_data_info, shader.handle(), name);
						if (UniformSetterUsageMode::Dynamic == usage_mode)
						{
							setter_funcs.emplace_back( [tex_unit = current_tex_unit](const Shader& shader, const MaterialData& mat, UniformSubSetter&)
							{
								if (mat.ambient_tex()) {
									shader.SetSampler(static_cast<GLuint>(MaterialUniformLocation::AmbientTexture), *mat.ambient_tex(), tex_unit);
								}
								else {
									shader.SetSampler(static_cast<GLuint>(MaterialUniformLocation::AmbientTexture), *MaterialData::GetOrCreateWhiteTex(), tex_unit);
								}
							} );
						}
						else // Static usage
						{
							assert(UniformSetterUsageMode::Static == usage_mode);
							setter_funcs.emplace_back( [tex_unit = current_tex_unit](const Shader& shader, const MaterialData& mat, UniformSubSetter& setter)
							{
								// static usage mode => texture never changes => replace the UniformSetter's function!
								if (mat.ambient_tex()) {
									setter.set_func([tex_unit](const Shader& shader, const MaterialData& mat, UniformSubSetter& setter) {
										shader.SetSampler(static_cast<GLuint>(MaterialUniformLocation::AmbientTexture), *mat.ambient_tex(), tex_unit);
									});
								}
								else {
									setter.set_func([tex_unit, white_tex = MaterialData::GetOrCreateWhiteTex()](const Shader& shader, const MaterialData& mat, UniformSubSetter& setter) {
										shader.SetSampler(static_cast<GLuint>(MaterialUniformLocation::AmbientTexture), *white_tex, tex_unit);
									});
								}
							} );
						}
						current_tex_unit += 1;
						break;
					case MaterialUniformLocation::EmissiveTexture:
						log_debug_verbose("In MaterialData[0x%p] for shader with handle[%u], emissive texture sampler is named [%s]", material_data_info, shader.handle(), name);
						if (UniformSetterUsageMode::Dynamic == usage_mode)
						{
							setter_funcs.emplace_back( [tex_unit = current_tex_unit](const Shader& shader, const MaterialData& mat, UniformSubSetter&)
							{
								if (mat.emissive_tex()) {
									shader.SetSampler(static_cast<GLuint>(MaterialUniformLocation::EmissiveTexture), *mat.emissive_tex(), tex_unit);
								}
								else {
									shader.SetSampler(static_cast<GLuint>(MaterialUniformLocation::EmissiveTexture), *MaterialData::GetOrCreateWhiteTex(), tex_unit);
								}
							} );
						}
						else // Static usage
						{
							assert(UniformSetterUsageMode::Static == usage_mode);
							setter_funcs.emplace_back( [tex_unit = current_tex_unit](const Shader& shader, const MaterialData& mat, UniformSubSetter& setter)
							{
								// static usage mode => texture never changes => replace the UniformSetter's function!
								if (mat.emissive_tex()) {
									setter.set_func([tex_unit](const Shader& shader, const MaterialData& mat, UniformSubSetter& setter) {
										shader.SetSampler(static_cast<GLuint>(MaterialUniformLocation::EmissiveTexture), *mat.emissive_tex(), tex_unit);
									});
								}
								else {
									setter.set_func([tex_unit, white_tex = MaterialData::GetOrCreateWhiteTex()](const Shader& shader, const MaterialData& mat, UniformSubSetter& setter) {
										shader.SetSampler(static_cast<GLuint>(MaterialUniformLocation::EmissiveTexture), *white_tex, tex_unit);
									});
								}
							} );
						}
						current_tex_unit += 1;
						break;
					case MaterialUniformLocation::HeightTexture:
						log_debug_verbose("In MaterialData[0x%p] for shader with handle[%u], height texture sampler is named [%s]", material_data_info, shader.handle(), name);
						if (UniformSetterUsageMode::Dynamic == usage_mode)
						{
							setter_funcs.emplace_back( [tex_unit = current_tex_unit](const Shader& shader, const MaterialData& mat, UniformSubSetter&)
							{
								if (mat.height_tex()) {
									shader.SetSampler(static_cast<GLuint>(MaterialUniformLocation::HeightTexture), *mat.height_tex(), tex_unit);
								}
								else {
									shader.SetSampler(static_cast<GLuint>(MaterialUniformLocation::HeightTexture), *MaterialData::GetOrCreateWhiteTex(), tex_unit);
								}
							} );
						}
						else // Static usage
						{
							assert(UniformSetterUsageMode::Static == usage_mode);
							setter_funcs.emplace_back( [tex_unit = current_tex_unit](const Shader& shader, const MaterialData& mat, UniformSubSetter& setter)
							{
								// static usage mode => texture never changes => replace the UniformSetter's function!
								if (mat.height_tex()) {
									setter.set_func([tex_unit](const Shader& shader, const MaterialData& mat, UniformSubSetter& setter) {
										shader.SetSampler(static_cast<GLuint>(MaterialUniformLocation::HeightTexture), *mat.height_tex(), tex_unit);
									});
								}
								else {
									setter.set_func([tex_unit, white_tex = MaterialData::GetOrCreateWhiteTex()](const Shader& shader, const MaterialData& mat, UniformSubSetter& setter) {
										shader.SetSampler(static_cast<GLuint>(MaterialUniformLocation::HeightTexture), *white_tex, tex_unit);
									});
								}
							} );
						}
						current_tex_unit += 1;
						break;
					case MaterialUniformLocation::NormalsTexture:
						log_debug_verbose("In MaterialData[0x%p] for shader with handle[%u], normals texture sampler is named [%s]", material_data_info, shader.handle(), name);
						if (UniformSetterUsageMode::Dynamic == usage_mode)
						{
							setter_funcs.emplace_back( [tex_unit = current_tex_unit](const Shader& shader, const MaterialData& mat, UniformSubSetter&)
							{
								if (mat.normals_tex()) {
									shader.SetSampler(static_cast<GLuint>(MaterialUniformLocation::NormalsTexture), *mat.normals_tex(), tex_unit);
								}
								else {
									shader.SetSampler(static_cast<GLuint>(MaterialUniformLocation::NormalsTexture), *MaterialData::GetOrCreateWhiteTex(), tex_unit);
								}
							} );
						}
						else // Static usage
						{
							assert(UniformSetterUsageMode::Static == usage_mode);
							setter_funcs.emplace_back( [tex_unit = current_tex_unit](const Shader& shader, const MaterialData& mat, UniformSubSetter& setter)
							{
								// static usage mode => texture never changes => replace the UniformSetter's function!
								if (mat.normals_tex()) {
									setter.set_func([tex_unit](const Shader& shader, const MaterialData& mat, UniformSubSetter& setter) {
										shader.SetSampler(static_cast<GLuint>(MaterialUniformLocation::NormalsTexture), *mat.normals_tex(), tex_unit);
									});
								}
								else {
									setter.set_func([tex_unit, white_tex = MaterialData::GetOrCreateWhiteTex()](const Shader& shader, const MaterialData& mat, UniformSubSetter& setter) {
										shader.SetSampler(static_cast<GLuint>(MaterialUniformLocation::NormalsTexture), *white_tex, tex_unit);
									});
								}
							} );
						}
						current_tex_unit += 1;
						break;
					case MaterialUniformLocation::ShininessTexture:
						log_debug_verbose("In MaterialData[0x%p] for shader with handle[%u], shininess texture sampler is named [%s]", material_data_info, shader.handle(), name);
						if (UniformSetterUsageMode::Dynamic == usage_mode)
						{
							setter_funcs.emplace_back( [tex_unit = current_tex_unit](const Shader& shader, const MaterialData& mat, UniformSubSetter&)
							{
								if (mat.shininess_tex()) {
									shader.SetSampler(static_cast<GLuint>(MaterialUniformLocation::ShininessTexture), *mat.shininess_tex(), tex_unit);
								}
								else {
									shader.SetSampler(static_cast<GLuint>(MaterialUniformLocation::ShininessTexture), *MaterialData::GetOrCreateWhiteTex(), tex_unit);
								}
							} );
						}
						else // Static usage
						{
							assert(UniformSetterUsageMode::Static == usage_mode);
							setter_funcs.emplace_back( [tex_unit = current_tex_unit](const Shader& shader, const MaterialData& mat, UniformSubSetter& setter)
							{
								// static usage mode => texture never changes => replace the UniformSetter's function!
								if (mat.shininess_tex()) {
									setter.set_func([tex_unit](const Shader& shader, const MaterialData& mat, UniformSubSetter& setter) {
										shader.SetSampler(static_cast<GLuint>(MaterialUniformLocation::ShininessTexture), *mat.shininess_tex(), tex_unit);
									});
								}
								else {
									setter.set_func([tex_unit, white_tex = MaterialData::GetOrCreateWhiteTex()](const Shader& shader, const MaterialData& mat, UniformSubSetter& setter) {
										shader.SetSampler(static_cast<GLuint>(MaterialUniformLocation::ShininessTexture), *white_tex, tex_unit);
									});
								}
							} );
						}
						current_tex_unit += 1;
						break;
					case MaterialUniformLocation::OpacityTexture:
						log_debug_verbose("In MaterialData[0x%p] for shader with handle[%u], opacity texture sampler is named [%s]", material_data_info, shader.handle(), name);
						if (UniformSetterUsageMode::Dynamic == usage_mode)
						{
							setter_funcs.emplace_back( [tex_unit = current_tex_unit](const Shader& shader, const MaterialData& mat, UniformSubSetter&)
							{
								if (mat.opacity_tex()) {
									shader.SetSampler(static_cast<GLuint>(MaterialUniformLocation::OpacityTexture), *mat.opacity_tex(), tex_unit);
								}
								else {
									shader.SetSampler(static_cast<GLuint>(MaterialUniformLocation::OpacityTexture), *MaterialData::GetOrCreateWhiteTex(), tex_unit);
								}
							} );
						}
						else // Static usage
						{
							assert(UniformSetterUsageMode::Static == usage_mode);
							setter_funcs.emplace_back( [tex_unit = current_tex_unit](const Shader& shader, const MaterialData& mat, UniformSubSetter& setter)
							{
								// static usage mode => texture never changes => replace the UniformSetter's function!
								if (mat.opacity_tex()) {
									setter.set_func([tex_unit](const Shader& shader, const MaterialData& mat, UniformSubSetter& setter) {
										shader.SetSampler(static_cast<GLuint>(MaterialUniformLocation::OpacityTexture), *mat.opacity_tex(), tex_unit);
									});
								}
								else {
									setter.set_func([tex_unit, white_tex = MaterialData::GetOrCreateWhiteTex()](const Shader& shader, const MaterialData& mat, UniformSubSetter& setter) {
										shader.SetSampler(static_cast<GLuint>(MaterialUniformLocation::OpacityTexture), *white_tex, tex_unit);
									});
								}
							} );
						}
						current_tex_unit += 1;
						break;
					case MaterialUniformLocation::DisplacementTexture:
						log_debug_verbose("In MaterialData[0x%p] for shader with handle[%u], displacement texture sampler is named [%s]", material_data_info, shader.handle(), name);
						if (UniformSetterUsageMode::Dynamic == usage_mode)
						{
							setter_funcs.emplace_back( [tex_unit = current_tex_unit](const Shader& shader, const MaterialData& mat, UniformSubSetter&)
							{
								if (mat.displacement_tex()) {
									shader.SetSampler(static_cast<GLuint>(MaterialUniformLocation::DisplacementTexture), *mat.displacement_tex(), tex_unit);
								}
								else {
									shader.SetSampler(static_cast<GLuint>(MaterialUniformLocation::DisplacementTexture), *MaterialData::GetOrCreateWhiteTex(), tex_unit);
								}
							} );
						}
						else // Static usage
						{
							assert(UniformSetterUsageMode::Static == usage_mode);
							setter_funcs.emplace_back( [tex_unit = current_tex_unit](const Shader& shader, const MaterialData& mat, UniformSubSetter& setter)
							{
								// static usage mode => texture never changes => replace the UniformSetter's function!
								if (mat.displacement_tex()) {
									setter.set_func([tex_unit](const Shader& shader, const MaterialData& mat, UniformSubSetter& setter) {
										shader.SetSampler(static_cast<GLuint>(MaterialUniformLocation::DisplacementTexture), *mat.displacement_tex(), tex_unit);
									});
								}
								else {
									setter.set_func([tex_unit, white_tex = MaterialData::GetOrCreateWhiteTex()](const Shader& shader, const MaterialData& mat, UniformSubSetter& setter) {
										shader.SetSampler(static_cast<GLuint>(MaterialUniformLocation::DisplacementTexture), *white_tex, tex_unit);
									});
								}
							} );
						}
						current_tex_unit += 1;
						break;
					case MaterialUniformLocation::ReflectionTexture:
						log_debug_verbose("In MaterialData[0x%p] for shader with handle[%u], reflection texture sampler is named [%s]", material_data_info, shader.handle(), name);
						if (UniformSetterUsageMode::Dynamic == usage_mode)
						{
							setter_funcs.emplace_back( [tex_unit = current_tex_unit](const Shader& shader, const MaterialData& mat, UniformSubSetter&)
							{
								if (mat.reflection_tex()) {
									shader.SetSampler(static_cast<GLuint>(MaterialUniformLocation::ReflectionTexture), *mat.reflection_tex(), tex_unit);
								}
								else {
									shader.SetSampler(static_cast<GLuint>(MaterialUniformLocation::ReflectionTexture), *MaterialData::GetOrCreateWhiteTex(), tex_unit);
								}
							} );
						}
						else // Static usage
						{
							assert(UniformSetterUsageMode::Static == usage_mode);
							setter_funcs.emplace_back( [tex_unit = current_tex_unit](const Shader& shader, const MaterialData& mat, UniformSubSetter& setter)
							{
								// static usage mode => texture never changes => replace the UniformSetter's function!
								if (mat.reflection_tex()) {
									setter.set_func([tex_unit](const Shader& shader, const MaterialData& mat, UniformSubSetter& setter) {
										shader.SetSampler(static_cast<GLuint>(MaterialUniformLocation::ReflectionTexture), *mat.reflection_tex(), tex_unit);
									});
								}
								else {
									setter.set_func([tex_unit, white_tex = MaterialData::GetOrCreateWhiteTex()](const Shader& shader, const MaterialData& mat, UniformSubSetter& setter) {
										shader.SetSampler(static_cast<GLuint>(MaterialUniformLocation::ReflectionTexture), *white_tex, tex_unit);
									});
								}
							} );
						}
						current_tex_unit += 1;
						break;
					case MaterialUniformLocation::LightmapTexture:
						log_debug_verbose("In MaterialData[0x%p] for shader with handle[%u], lightmap texture sampler is named [%s]", material_data_info, shader.handle(), name);
						if (UniformSetterUsageMode::Dynamic == usage_mode)
						{
							setter_funcs.emplace_back( [tex_unit = current_tex_unit](const Shader& shader, const MaterialData& mat, UniformSubSetter&)
							{
								if (mat.lightmap_tex()) {
									shader.SetSampler(static_cast<GLuint>(MaterialUniformLocation::LightmapTexture), *mat.lightmap_tex(), tex_unit);
								}
								else {
									shader.SetSampler(static_cast<GLuint>(MaterialUniformLocation::LightmapTexture), *MaterialData::GetOrCreateWhiteTex(), tex_unit);
								}
							} );
						}
						else // Static usage
						{
							assert(UniformSetterUsageMode::Static == usage_mode);
							setter_funcs.emplace_back( [tex_unit = current_tex_unit](const Shader& shader, const MaterialData& mat, UniformSubSetter& setter)
							{
								// static usage mode => texture never changes => replace the UniformSetter's function!
								if (mat.lightmap_tex()) {
									setter.set_func([tex_unit](const Shader& shader, const MaterialData& mat, UniformSubSetter& setter) {
										shader.SetSampler(static_cast<GLuint>(MaterialUniformLocation::LightmapTexture), *mat.lightmap_tex(), tex_unit);
									});
								}
								else {
									setter.set_func([tex_unit, white_tex = MaterialData::GetOrCreateWhiteTex()](const Shader& shader, const MaterialData& mat, UniformSubSetter& setter) {
										shader.SetSampler(static_cast<GLuint>(MaterialUniformLocation::LightmapTexture), *white_tex, tex_unit);
									});
								}
							} );
						}
						current_tex_unit += 1;
						break;
					case MaterialUniformLocation::Albedo:
						log_debug_verbose("In MaterialData[0x%p] for shader with handle[%u], albedo uniform is named [%s]", material_data_info, shader.handle(), name);
						setter_funcs.emplace_back( [](const Shader& shader, const MaterialData& mat, UniformSubSetter&)
						{
							shader.SetUniform(static_cast<GLuint>(MaterialUniformLocation::Albedo), mat.albedo());
						} );
						break;
						break;
					case MaterialUniformLocation::Metallic:
						log_debug_verbose("In MaterialData[0x%p] for shader with handle[%u], metallic uniform is named [%s]", material_data_info, shader.handle(), name);
						setter_funcs.emplace_back( [](const Shader& shader, const MaterialData& mat, UniformSubSetter&)
						{
							shader.SetUniform(static_cast<GLuint>(MaterialUniformLocation::Metallic), mat.metallic());
						} );
						break;
					case MaterialUniformLocation::Smoothness:
						log_debug_verbose("In MaterialData[0x%p] for shader with handle[%u], smoothness uniform is named [%s]", material_data_info, shader.handle(), name);
						setter_funcs.emplace_back( [](const Shader& shader, const MaterialData& mat, UniformSubSetter&)
						{
							shader.SetUniform(static_cast<GLuint>(MaterialUniformLocation::Smoothness), mat.smoothness());
						} );
						break;
					case MaterialUniformLocation::Sheen:
						log_debug_verbose("In MaterialData[0x%p] for shader with handle[%u], sheen uniform is named [%s]", material_data_info, shader.handle(), name);
						setter_funcs.emplace_back( [](const Shader& shader, const MaterialData& mat, UniformSubSetter&)
						{
							shader.SetUniform(static_cast<GLuint>(MaterialUniformLocation::Sheen), mat.sheen());
						} );
						break;
					case MaterialUniformLocation::Thickness:
						log_debug_verbose("In MaterialData[0x%p] for shader with handle[%u], thickness uniform is named [%s]", material_data_info, shader.handle(), name);
						setter_funcs.emplace_back( [](const Shader& shader, const MaterialData& mat, UniformSubSetter&)
						{
							shader.SetUniform(static_cast<GLuint>(MaterialUniformLocation::Thickness), mat.thickness());
						} );
						break;
					case MaterialUniformLocation::Roughness:
						log_debug_verbose("In MaterialData[0x%p] for shader with handle[%u], roughness uniform is named [%s]", material_data_info, shader.handle(), name);
						setter_funcs.emplace_back( [](const Shader& shader, const MaterialData& mat, UniformSubSetter&)
						{
							shader.SetUniform(static_cast<GLuint>(MaterialUniformLocation::Roughness), mat.roughness());
						} );
						break;
					case MaterialUniformLocation::Anisotropy:
						log_debug_verbose("In MaterialData[0x%p] for shader with handle[%u], anisotropy uniform is named [%s]", material_data_info, shader.handle(), name);
						setter_funcs.emplace_back( [](const Shader& shader, const MaterialData& mat, UniformSubSetter&)
						{
							shader.SetUniform(static_cast<GLuint>(MaterialUniformLocation::Anisotropy), mat.anisotropy());
						} );
						break;
					case MaterialUniformLocation::AnisotropyRotation:
						log_debug_verbose("In MaterialData[0x%p] for shader with handle[%u], anisotropy-rotation uniform is named [%s]", material_data_info, shader.handle(), name);
						setter_funcs.emplace_back( [](const Shader& shader, const MaterialData& mat, UniformSubSetter&)
						{
							shader.SetUniform(static_cast<GLuint>(MaterialUniformLocation::AnisotropyRotation), mat.anisotropy_rotation());
						} );
						break;
					case MaterialUniformLocation::Offset:
						log_debug_verbose("In MaterialData[0x%p] for shader with handle[%u], offset uniform is named [%s]", material_data_info, shader.handle(), name);
						setter_funcs.emplace_back( [](const Shader& shader, const MaterialData& mat, UniformSubSetter&)
						{
							shader.SetUniform(static_cast<GLuint>(MaterialUniformLocation::Offset), mat.offset());
						} );
						break;
					case MaterialUniformLocation::Tiling:
						log_debug_verbose("In MaterialData[0x%p] for shader with handle[%u], tiling uniform is named [%s]", material_data_info, shader.handle(), name);
						setter_funcs.emplace_back( [](const Shader& shader, const MaterialData& mat, UniformSubSetter&)
						{
							shader.SetUniform(static_cast<GLuint>(MaterialUniformLocation::Tiling), mat.tiling());
						} );
						break;
				}
			}
		}

		return UniformSetter(shader, [n = setter_funcs.size(), setters = std::move(setter_funcs)] (const Shader& shader, const MaterialData& mat) mutable
		{
			for (auto i = 0; i < n; ++i)
			{
				setters[i](shader, mat);
			}
		}, usage_mode);
	}

}
