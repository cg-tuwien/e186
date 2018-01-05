#pragma once
#include "Tex2D.h"
#include "TexLoadingHelper.h"

namespace e186
{
	enum BlendMode : unsigned int
	{
		BlendMode_AlphaBlended,
		BlendMode_Additive
	};

	UniformSetter CreateUniformSetterForShader(const Shader& shader, MaterialData& material_data);

	class MaterialData
	{
		friend UniformSetter CreateUniformSetterForShader(const Shader& shader, MaterialData& material_data);

	public:
		MaterialData();
		/*! Initialize from an Assimp-mesh */
		MaterialData(aiMaterial* aimat, TexLoadingHelper& tlh);
		MaterialData(MaterialData&&) = default;
		MaterialData(const MaterialData&) = default;
		MaterialData& operator= (MaterialData&&) = default;
		MaterialData& operator= (const MaterialData&) = default;
		~MaterialData() = default;

		const std::string& name() const { return m_name;  }
		const glm::vec3& diffuse_color() const { return m_diffuse_color; }
		const glm::vec3& specular_color() const { return m_specular_color; }
		const glm::vec3& ambient_color() const { return m_ambient_color; }
		const glm::vec3& emissive_color() const { return m_emissive_color; }
		const glm::vec3& transparent_color() const { return m_transparent_color; }
		bool wireframe_mode() const { return m_wireframe_mode; }
		bool twosided() const { return m_twosided; }
		BlendMode blend_mode() const { return m_blend_mode; }
		float opacity() const { return m_opacity; }
		float shininess() const { return m_shininess; }
		float shininess_strength() const { return m_shininess_strength; }
		float refraction_index() const { return m_refraction_index; }
		float reflectivity() const { return m_reflectivity; }
		const std::shared_ptr<Tex2D>& diffuse_tex() const { return m_diffuse_tex; }
		const std::shared_ptr<Tex2D>& specular_tex() const { return m_specular_tex; }
		const std::shared_ptr<Tex2D>& ambient_tex() const { return m_ambient_tex; }
		const std::shared_ptr<Tex2D>& emissive_tex() const { return m_emissive_tex; }
		const std::shared_ptr<Tex2D>& height_tex() const { return m_height_tex; }
		const std::shared_ptr<Tex2D>& normals_tex() const { return m_normals_tex; }
		const std::shared_ptr<Tex2D>& shininess_tex() const { return m_shininess_tex; }
		const std::shared_ptr<Tex2D>& opacity_tex() const { return m_opacity_tex; }
		const std::shared_ptr<Tex2D>& displacement_tex() const { return m_displacement_tex; }
		const std::shared_ptr<Tex2D>& reflection_tex() const { return m_reflection_tex; }
		const std::shared_ptr<Tex2D>& lightmap_tex() const { return m_lightmap_tex; }

	private:
		std::string m_name;

		glm::vec3 m_diffuse_color;
		glm::vec3 m_specular_color;
		glm::vec3 m_ambient_color;
		glm::vec3 m_emissive_color;
		glm::vec3 m_transparent_color;
		bool m_wireframe_mode;
		bool m_twosided;
		BlendMode m_blend_mode;
		float m_opacity;
		float m_shininess;
		float m_shininess_strength;
		float m_refraction_index;
		float m_reflectivity;
		std::shared_ptr<Tex2D> m_diffuse_tex;
		std::shared_ptr<Tex2D> m_specular_tex;
		std::shared_ptr<Tex2D> m_ambient_tex;
		std::shared_ptr<Tex2D> m_emissive_tex;
		std::shared_ptr<Tex2D> m_height_tex;
		std::shared_ptr<Tex2D> m_normals_tex;
		std::shared_ptr<Tex2D> m_shininess_tex;
		std::shared_ptr<Tex2D> m_opacity_tex;
		std::shared_ptr<Tex2D> m_displacement_tex;
		std::shared_ptr<Tex2D> m_reflection_tex;
		std::shared_ptr<Tex2D> m_lightmap_tex;

		static TexParams ai_mapping_mode_to_tex_params(aiTextureMapMode aimm);
	};

}
