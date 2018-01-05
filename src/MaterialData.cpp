#include "MaterialData.h"
#include "MaterialUniformLocation.h"

namespace e186
{
	MaterialData::MaterialData() :
		m_name(""),
		m_diffuse_color(1.0f, 1.0f, 1.0f),
		m_specular_color(1.0f, 1.0f, 1.0f),
		m_ambient_color(1.0f, 1.0f, 1.0f),
		m_emissive_color(1.0f, 1.0f, 1.0f),
		m_transparent_color(1.0f, 1.0f, 1.0f),
		m_wireframe_mode(false),
		m_twosided(false),
		m_blend_mode(BlendMode_AlphaBlended),
		m_opacity(1.0f),
		m_shininess(0.0f),
		m_shininess_strength(0.0f),
		m_refraction_index(0.0f),
		m_reflectivity(0.0f)
	{ 
	}

	TexParams MaterialData::ai_mapping_mode_to_tex_params(aiTextureMapMode aimm)
	{
		switch (aimm)
		{
		case aiTextureMapMode_Wrap:
			return TexParams_Repeat;
		case aiTextureMapMode_Clamp:
			return TexParams_ClampToEdge;
		case aiTextureMapMode_Decal:
			return TexParams_ClampToEdge;
		case aiTextureMapMode_Mirror:
			return TexParams_MirroredRepeat;
		default:
			return TexParams_Repeat;
		}
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
			m_diffuse_color = glm::vec3(color.r, color.g, color.b);
		}

		if (AI_SUCCESS == aimat->Get(AI_MATKEY_COLOR_SPECULAR, color)) {
			m_specular_color = glm::vec3(color.r, color.g, color.b);
		}

		if (AI_SUCCESS == aimat->Get(AI_MATKEY_COLOR_AMBIENT, color)) {
			m_ambient_color = glm::vec3(color.r, color.g, color.b);
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
				? BlendMode_Additive
				: BlendMode_AlphaBlended;
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
			auto tex = tlh.GetOrLoadTex(strVal.data, tlh.general_tex_params() | ai_mapping_mode_to_tex_params(texMapMode) | TexParams_GenerateMipMaps);
			m_diffuse_tex = tex;
		}

		if (AI_SUCCESS == aimat->GetTexture(aiTextureType_SPECULAR, 0, &strVal, &texMapping, nullptr, nullptr, nullptr, &texMapMode)) {
			if (texMapping != aiTextureMapping_UV) {
				log_warning("Unsupported texture mapping mode[%u] for texture[%s]", static_cast<unsigned int>(texMapping), strVal.C_Str());
			}
			auto tex = tlh.GetOrLoadTex(strVal.data, tlh.general_tex_params() | ai_mapping_mode_to_tex_params(texMapMode) | TexParams_GenerateMipMaps);
			m_specular_tex = tex;
		}

		if (AI_SUCCESS == aimat->GetTexture(aiTextureType_AMBIENT, 0, &strVal, &texMapping, nullptr, nullptr, nullptr, &texMapMode)) {
			if (texMapping != aiTextureMapping_UV) {
				log_warning("Unsupported texture mapping mode[%u] for texture[%s]", static_cast<unsigned int>(texMapping), strVal.C_Str());
			}
			auto tex = tlh.GetOrLoadTex(strVal.data, tlh.general_tex_params() | ai_mapping_mode_to_tex_params(texMapMode) | TexParams_GenerateMipMaps);
			m_ambient_tex = tex;
		}

		if (AI_SUCCESS == aimat->GetTexture(aiTextureType_EMISSIVE, 0, &strVal, &texMapping, nullptr, nullptr, nullptr, &texMapMode)) {
			if (texMapping != aiTextureMapping_UV) {
				log_warning("Unsupported texture mapping mode[%u] for texture[%s]", static_cast<unsigned int>(texMapping), strVal.C_Str());
			}
			auto tex = tlh.GetOrLoadTex(strVal.data, tlh.general_tex_params() | ai_mapping_mode_to_tex_params(texMapMode) | TexParams_GenerateMipMaps);
			m_emissive_tex = tex;
		}

		if (AI_SUCCESS == aimat->GetTexture(aiTextureType_HEIGHT, 0, &strVal, &texMapping, nullptr, nullptr, nullptr, &texMapMode)) {
			if (texMapping != aiTextureMapping_UV) {
				log_warning("Unsupported texture mapping mode[%u] for texture[%s]", static_cast<unsigned int>(texMapping), strVal.C_Str());
			}
			auto tex = tlh.GetOrLoadTex(strVal.data, tlh.general_tex_params() | ai_mapping_mode_to_tex_params(texMapMode) | TexParams_GenerateMipMaps);
			m_height_tex = tex;
		}

		if (AI_SUCCESS == aimat->GetTexture(aiTextureType_NORMALS, 0, &strVal, &texMapping, nullptr, nullptr, nullptr, &texMapMode)) {
			if (texMapping != aiTextureMapping_UV) {
				log_warning("Unsupported texture mapping mode[%u] for texture[%s]", static_cast<unsigned int>(texMapping), strVal.C_Str());
			}
			auto tex = tlh.GetOrLoadTex(strVal.data, tlh.general_tex_params() | ai_mapping_mode_to_tex_params(texMapMode));
			m_normals_tex = tex;
		}

		if (AI_SUCCESS == aimat->GetTexture(aiTextureType_SHININESS, 0, &strVal, &texMapping, nullptr, nullptr, nullptr, &texMapMode)) {
			if (texMapping != aiTextureMapping_UV) {
				log_warning("Unsupported texture mapping mode[%u] for texture[%s]", static_cast<unsigned int>(texMapping), strVal.C_Str());
			}
			auto tex = tlh.GetOrLoadTex(strVal.data, tlh.general_tex_params() | ai_mapping_mode_to_tex_params(texMapMode) | TexParams_GenerateMipMaps);
			m_shininess_tex = tex;
		}

		if (AI_SUCCESS == aimat->GetTexture(aiTextureType_OPACITY, 0, &strVal, &texMapping, nullptr, nullptr, nullptr, &texMapMode)) {
			if (texMapping != aiTextureMapping_UV) {
				log_warning("Unsupported texture mapping mode[%u] for texture[%s]", static_cast<unsigned int>(texMapping), strVal.C_Str());
			}
			auto tex = tlh.GetOrLoadTex(strVal.data, tlh.general_tex_params() | ai_mapping_mode_to_tex_params(texMapMode) | TexParams_GenerateMipMaps);
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
			auto tex = tlh.GetOrLoadTex(strVal.data, tlh.general_tex_params() | ai_mapping_mode_to_tex_params(texMapMode) | TexParams_GenerateMipMaps);
			m_reflection_tex = tex;
		}

		if (AI_SUCCESS == aimat->GetTexture(aiTextureType_LIGHTMAP, 0, &strVal, &texMapping, nullptr, nullptr, nullptr, &texMapMode)) {
			if (texMapping != aiTextureMapping_UV) {
				log_warning("Unsupported texture mapping mode[%u] for texture[%s]", static_cast<unsigned int>(texMapping), strVal.C_Str());
			}
			auto tex = tlh.GetOrLoadTex(strVal.data, tlh.general_tex_params() | ai_mapping_mode_to_tex_params(texMapMode) | TexParams_GenerateMipMaps);
			m_lightmap_tex = tex;
		}
	}

	UniformSetter CreateUniformSetterForShader(const Shader& shader, MaterialData& material_data)
	{
		shader.Use();

		std::vector<std::function<void(const Shader&, const MaterialData&)>> setter_funcs;

		GLchar name[GL_ACTIVE_UNIFORM_MAX_LENGTH];
		*name = 0;
		GLsizei name_len = 0;
		GLint size_of_the_uniform_variable = 0;
		GLenum type_of_the_uniform_variable = 0;

		GLuint current_texture_unit = 0;

		auto white_tex = std::make_shared<Tex2D>();
		
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
						log_debug_verbose("In MaterialData[0x%p] for shader with handle[%u], diffuse color uniform is named [%s]", &material_data, shader.handle(), name);
						setter_funcs.push_back([](const Shader& shdr, const MaterialData& mat)
						{
							shdr.SetUniform(static_cast<GLuint>(MaterialUniformLocation::DiffuseColor), mat.diffuse_color());
						});
						break;
					case MaterialUniformLocation::SpecularColor:
						log_debug_verbose("In MaterialData[0x%p] for shader with handle[%u], specular color uniform is named [%s]", &material_data, shader.handle(), name);
						setter_funcs.push_back([](const Shader& shdr, const MaterialData& mat)
						{
							shdr.SetUniform(static_cast<GLuint>(MaterialUniformLocation::SpecularColor), mat.specular_color());
						});
						break;
					case MaterialUniformLocation::AmbientColor:
						log_debug_verbose("In MaterialData[0x%p] for shader with handle[%u], ambient color uniform is named [%s]", &material_data, shader.handle(), name);
						setter_funcs.push_back([](const Shader& shdr, const MaterialData& mat)
						{
							shdr.SetUniform(static_cast<GLuint>(MaterialUniformLocation::AmbientColor), mat.ambient_color());
						});
						break;
					case MaterialUniformLocation::EmissiveColor:
						log_debug_verbose("In MaterialData[0x%p] for shader with handle[%u], emissive color uniform is named [%s]", &material_data, shader.handle(), name);
						setter_funcs.push_back([](const Shader& shdr, const MaterialData& mat)
						{
							shdr.SetUniform(static_cast<GLuint>(MaterialUniformLocation::EmissiveColor), mat.emissive_color());
						});
						break;
					case MaterialUniformLocation::TransparentColor:
						log_debug_verbose("In MaterialData[0x%p] for shader with handle[%u], transparent color uniform is named [%s]", &material_data, shader.handle(), name);
						setter_funcs.push_back([](const Shader& shdr, const MaterialData& mat)
						{
							shdr.SetUniform(static_cast<GLuint>(MaterialUniformLocation::TransparentColor), mat.transparent_color());
						});
						break;
					case MaterialUniformLocation::Opacity:
						log_debug_verbose("In MaterialData[0x%p] for shader with handle[%u], opacity uniform is named [%s]", &material_data, shader.handle(), name);
						setter_funcs.push_back([](const Shader& shdr, const MaterialData& mat)
						{
							shdr.SetUniform(static_cast<GLuint>(MaterialUniformLocation::Opacity), mat.opacity());
						});
						break;
					case MaterialUniformLocation::Shininess:
						log_debug_verbose("In MaterialData[0x%p] for shader with handle[%u], shininess uniform is named [%s]", &material_data, shader.handle(), name);
						setter_funcs.push_back([](const Shader& shdr, const MaterialData& mat)
						{
							shdr.SetUniform(static_cast<GLuint>(MaterialUniformLocation::Shininess), mat.shininess());
						});
						break;
					case MaterialUniformLocation::ShininessStrength:
						log_debug_verbose("In MaterialData[0x%p] for shader with handle[%u], shininess strength uniform is named [%s]", &material_data, shader.handle(), name);
						setter_funcs.push_back([](const Shader& shdr, const MaterialData& mat)
						{
							shdr.SetUniform(static_cast<GLuint>(MaterialUniformLocation::ShininessStrength), mat.shininess_strength());
						});
						break;
					case MaterialUniformLocation::RefractionIndex:
						log_debug_verbose("In MaterialData[0x%p] for shader with handle[%u], refraction index uniform is named [%s]", &material_data, shader.handle(), name);
						setter_funcs.push_back([](const Shader& shdr, const MaterialData& mat)
						{
							shdr.SetUniform(static_cast<GLuint>(MaterialUniformLocation::RefractionIndex), mat.refraction_index());
						});
						break;
					case MaterialUniformLocation::Reflectivity:
						log_debug_verbose("In MaterialData[0x%p] for shader with handle[%u], reflectivity uniform is named [%s]", &material_data, shader.handle(), name);
						setter_funcs.push_back([](const Shader& shdr, const MaterialData& mat)
						{
							shdr.SetUniform(static_cast<GLuint>(MaterialUniformLocation::Reflectivity), mat.reflectivity());
						});
						break;
					case MaterialUniformLocation::DiffuseTexture:
						log_debug_verbose("In MaterialData[0x%p] for shader with handle[%u], diffuse texture sampler is named [%s]", &material_data, shader.handle(), name);
						if (!material_data.m_diffuse_tex)
						{
							material_data.m_diffuse_tex = white_tex;
						}
						setter_funcs.push_back([tex_unit = current_texture_unit++](const Shader& shdr, const MaterialData& mat)
						{
							shdr.SetSampler(static_cast<GLuint>(MaterialUniformLocation::DiffuseTexture), *mat.diffuse_tex(), tex_unit);
						});
						break;
					case MaterialUniformLocation::SpecularTexture:
						log_debug_verbose("In MaterialData[0x%p] for shader with handle[%u], specular texture sampler is named [%s]", &material_data, shader.handle(), name);
						if (!material_data.m_specular_tex)
						{
							material_data.m_specular_tex = white_tex;
						}
						setter_funcs.push_back([tex_unit = current_texture_unit++](const Shader& shdr, const MaterialData& mat)
						{
							shdr.SetSampler(static_cast<GLuint>(MaterialUniformLocation::SpecularTexture), *mat.specular_tex(), tex_unit);
						});
						break;
					case MaterialUniformLocation::AmbientTexture:
						log_debug_verbose("In MaterialData[0x%p] for shader with handle[%u], ambient texture sampler is named [%s]", &material_data, shader.handle(), name);
						if (!material_data.m_ambient_tex)
						{
							material_data.m_ambient_tex = white_tex;
						}
						setter_funcs.push_back([tex_unit = current_texture_unit++](const Shader& shdr, const MaterialData& mat)
						{
							shdr.SetSampler(static_cast<GLuint>(MaterialUniformLocation::AmbientTexture), *mat.ambient_tex(), tex_unit);
						});
						break;
					case MaterialUniformLocation::EmissiveTexture:
						log_debug_verbose("In MaterialData[0x%p] for shader with handle[%u], emissive texture sampler is named [%s]", &material_data, shader.handle(), name);
						if (!material_data.m_emissive_tex)
						{
							material_data.m_emissive_tex = white_tex;
						}
						setter_funcs.push_back([tex_unit = current_texture_unit++](const Shader& shdr, const MaterialData& mat)
						{
							shdr.SetSampler(static_cast<GLuint>(MaterialUniformLocation::EmissiveTexture), *mat.emissive_tex(), tex_unit);
						});
						break;
					case MaterialUniformLocation::HeightTexture:
						log_debug_verbose("In MaterialData[0x%p] for shader with handle[%u], height texture sampler is named [%s]", &material_data, shader.handle(), name);
						if (!material_data.m_height_tex)
						{
							material_data.m_height_tex = white_tex;
						}
						setter_funcs.push_back([tex_unit = current_texture_unit++](const Shader& shdr, const MaterialData& mat)
						{
							shdr.SetSampler(static_cast<GLuint>(MaterialUniformLocation::HeightTexture), *mat.height_tex(), tex_unit);
						});
						break;
					case MaterialUniformLocation::NormalsTexture:
						log_debug_verbose("In MaterialData[0x%p] for shader with handle[%u], normals texture sampler is named [%s]", &material_data, shader.handle(), name);
						if (!material_data.m_normals_tex)
						{
							material_data.m_normals_tex = white_tex;
						}
						setter_funcs.push_back([tex_unit = current_texture_unit++](const Shader& shdr, const MaterialData& mat)
						{
							shdr.SetSampler(static_cast<GLuint>(MaterialUniformLocation::NormalsTexture), *mat.normals_tex(), tex_unit);
						});
						break;
					case MaterialUniformLocation::ShininessTexture:
						log_debug_verbose("In MaterialData[0x%p] for shader with handle[%u], shininess texture sampler is named [%s]", &material_data, shader.handle(), name);
						if (!material_data.m_shininess_tex)
						{
							material_data.m_shininess_tex = white_tex;
						}
						setter_funcs.push_back([tex_unit = current_texture_unit++](const Shader& shdr, const MaterialData& mat)
						{
							shdr.SetSampler(static_cast<GLuint>(MaterialUniformLocation::ShininessTexture), *mat.shininess_tex(), tex_unit);
						});
						break;
					case MaterialUniformLocation::OpacityTexture:
						log_debug_verbose("In MaterialData[0x%p] for shader with handle[%u], opacity texture sampler is named [%s]", &material_data, shader.handle(), name);
						if (!material_data.m_opacity_tex)
						{
							material_data.m_opacity_tex = white_tex;
						}
						setter_funcs.push_back([tex_unit = current_texture_unit++](const Shader& shdr, const MaterialData& mat)
						{
							shdr.SetSampler(static_cast<GLuint>(MaterialUniformLocation::OpacityTexture), *mat.opacity_tex(), tex_unit);
						});
						break;
					case MaterialUniformLocation::DisplacementTexture:
						log_debug_verbose("In MaterialData[0x%p] for shader with handle[%u], displacement texture sampler is named [%s]", &material_data, shader.handle(), name);
						if (!material_data.m_displacement_tex)
						{
							material_data.m_displacement_tex = white_tex;
						}
						setter_funcs.push_back([tex_unit = current_texture_unit++](const Shader& shdr, const MaterialData& mat)
						{
							shdr.SetSampler(static_cast<GLuint>(MaterialUniformLocation::DisplacementTexture), *mat.displacement_tex(), tex_unit);
						});
						break;
					case MaterialUniformLocation::ReflectionTexture:
						log_debug_verbose("In MaterialData[0x%p] for shader with handle[%u], reflection texture sampler is named [%s]", &material_data, shader.handle(), name);
						if (!material_data.m_reflection_tex)
						{
							material_data.m_reflection_tex = white_tex;
						}
						setter_funcs.push_back([tex_unit = current_texture_unit++](const Shader& shdr, const MaterialData& mat)
						{
							shdr.SetSampler(static_cast<GLuint>(MaterialUniformLocation::ReflectionTexture), *mat.reflection_tex(), tex_unit);
						});
						break;
					case MaterialUniformLocation::LightmapTexture:
						log_debug_verbose("In MaterialData[0x%p] for shader with handle[%u], lightmap texture sampler is named [%s]", &material_data, shader.handle(), name);
						if (!material_data.m_lightmap_tex)
						{
							material_data.m_lightmap_tex = white_tex;
						}
						setter_funcs.push_back([tex_unit = current_texture_unit++](const Shader& shdr, const MaterialData& mat)
						{
							shdr.SetSampler(static_cast<GLuint>(MaterialUniformLocation::LightmapTexture), *mat.lightmap_tex(), tex_unit);
						});
						break;
				}
			}
		}

		// Upload white texture only if we have used it!
		if (white_tex.use_count() > 1)
		{
			white_tex->Generate1pxWhite().Upload().SetTextureParameters(TexParams::TexParams_TrilinearFiltering | TexParams_ClampToEdge);
		}
		
		return[n = setter_funcs.size(), setters = std::move(setter_funcs)](const Shader& shdr, const MaterialData& mat)
		{
			for (auto i = 0; i < n; ++i)
			{
				setters[i](shdr, mat);
			}
		};
	}

}
