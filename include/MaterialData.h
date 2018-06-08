#pragma once
#include "Tex2D.h"
#include "TexLoadingHelper.h"

namespace e186
{
	enum struct BlendMode : uint32_t
	{
		AlphaBlended,
		Additive
	};

	enum struct UniformSetterUsageMode : uint32_t
	{
		Static,
		Dynamic
	};

	UniformSetter CreateUniformSetterForShader(Shader& shader, MaterialData& material_data_info);
	UniformSetter CreateUniformSetterForShader(Shader& shader, UniformSetterUsageMode usage_mode = UniformSetterUsageMode::Static, MaterialData* material_data_info = nullptr);

	class MaterialData
	{
		friend UniformSetter CreateUniformSetterForShader(Shader&, MaterialData&);
		friend UniformSetter CreateUniformSetterForShader(Shader&, UniformSetterUsageMode, MaterialData*);

	public:
		static const std::shared_ptr<Tex2D>& GetOrCreateWhiteTex();
		static const std::shared_ptr<Tex2D>& GetOrCreateBlackTex();
		static const std::shared_ptr<Tex2D>& GetOrCreateStraightUpNormalTex();

		MaterialData();
		/*! Initialize from an Assimp-mesh */
		MaterialData(aiMaterial* aimat, TexLoadingHelper& tlh);
		MaterialData(MaterialData&&) = default;
		MaterialData(const MaterialData&) = default;
		MaterialData& operator= (MaterialData&&) = default;
		MaterialData& operator= (const MaterialData&) = default;
		~MaterialData() = default;

		const std::string& name() const { return m_name;  }
		bool is_hidden() const { return m_hidden; }
		const glm::vec3& diffuse_reflectivity() const { return m_diffuse_reflectivity; }
		const glm::vec3& specular_reflectivity() const { return m_specular_reflectivity; }
		const glm::vec3& ambient_reflectivity() const { return m_ambient_reflectivity; }
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
		const glm::vec3& albedo() const { return m_albedo; }
		float metallic() const { return m_metallic; }
		float smoothness() const { return m_smoothness; }
		float sheen() const { return m_sheen; }
		float thickness() const { return m_thickness; }
		float roughness() const { return m_roughness; }
		float anisotropy() const { return m_anisotropy; }
		const glm::vec3& anisotropy_rotation() const { return m_anisotropy_rotation; }
		const glm::vec2& offset() const { return m_offset; }
		const glm::vec2& tiling() const { return m_tiling; }

		void set_name(std::string value) { m_name = std::move(value); }
		void set_is_hidden(bool is_hidden) { m_hidden = is_hidden; }
		void set_diffuse_reflectivity(const glm::vec3& value) { m_diffuse_reflectivity = value; }
		void set_specular_reflectivity(const glm::vec3& value) { m_specular_reflectivity = value; }
		void set_ambient_reflectivity(const glm::vec3& value) { m_ambient_reflectivity = value; }
		void set_emissive_color(const glm::vec3& value) { m_emissive_color = value; }
		void set_transparent_color(const glm::vec3& value) { m_transparent_color = value; }
		void set_wireframe_mode(bool enabled) { m_wireframe_mode = enabled; }
		void set_twosided(bool enabled) { m_twosided = enabled; }
		void set_blend_mode(BlendMode value) { m_blend_mode = value; }
		void set_opacity(float value) { m_opacity = value; }
		void set_shininess(float value) { m_shininess = value; }
		void set_shininess_strength(float value) { m_shininess_strength = value; }
		void set_refraction_index(float value) { m_refraction_index = value; }
		void set_reflectivity(float value) { m_reflectivity = value; }
		void set_diffuse_tex     (std::shared_ptr<Tex2D> tex) { m_diffuse_tex      = std::move(tex); }
		void set_specular_tex    (std::shared_ptr<Tex2D> tex) { m_specular_tex     = std::move(tex); }
		void set_ambient_tex     (std::shared_ptr<Tex2D> tex) { m_ambient_tex      = std::move(tex); }
		void set_emissive_tex    (std::shared_ptr<Tex2D> tex) { m_emissive_tex     = std::move(tex); }
		void set_height_tex      (std::shared_ptr<Tex2D> tex) { m_height_tex       = std::move(tex); }
		void set_normals_tex     (std::shared_ptr<Tex2D> tex) { m_normals_tex      = std::move(tex); }
		void set_shininess_tex   (std::shared_ptr<Tex2D> tex) { m_shininess_tex    = std::move(tex); }
		void set_opacity_tex     (std::shared_ptr<Tex2D> tex) { m_opacity_tex      = std::move(tex); }
		void set_displacement_tex(std::shared_ptr<Tex2D> tex) { m_displacement_tex = std::move(tex); }
		void set_reflection_tex  (std::shared_ptr<Tex2D> tex) { m_reflection_tex   = std::move(tex); }
		void set_lightmap_tex    (std::shared_ptr<Tex2D> tex) { m_lightmap_tex     = std::move(tex); }
		void set_albedo(const glm::vec3& value) { m_albedo = value; }
		void set_metallic(float value) { m_metallic = value; }
		void set_smoothness(float value) { m_smoothness = value; }
		void set_sheen(float value) { m_sheen = value; }
		void set_thickness(float value) { m_thickness = value; }
		void set_roughness(float value) { m_roughness = value; }
		void set_anisotropy(float value) { m_anisotropy = value; }
		void set_anisotropy_rotation(const glm::vec3& value) { m_anisotropy_rotation = value; }
		void set_offset(const glm::vec2& value) { m_offset = value; }
		void set_tiling(const glm::vec2& value) { m_tiling = value; }

	private:
		// The first two are not uploaded to the shader, i.e. no uniform setters generated for these 
		std::string m_name;
		bool m_hidden;
		// All of the following properties can be uploaded to the shader, if the respective locations 
		// are defined in the shader; i.e. uniform setters can be generated for all of the following:
		glm::vec3 m_diffuse_reflectivity;
		glm::vec3 m_specular_reflectivity;
		glm::vec3 m_ambient_reflectivity;
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
		glm::vec3 m_albedo;
		float m_metallic;
		float m_smoothness;
		float m_sheen;
		float m_thickness;
		float m_roughness;
		float m_anisotropy;
		glm::vec3 m_anisotropy_rotation;
		glm::vec2 m_offset;
		glm::vec2 m_tiling;

		static TexParams ai_mapping_mode_to_tex_params(aiTextureMapMode aimm);
	};

}
