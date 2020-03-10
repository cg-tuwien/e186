#pragma once
namespace e186
{
	/*! Vertex Attribute locations
	*/
	enum class MaterialUniformLocation : unsigned int
	{
		/*! Diffuse color at layout (location = 110) -- type: vec3 */
		DiffuseColor = 110,

		/*! Specular color at layout (location = 111) -- type: vec3 */
		SpecularColor,

		/*! Ambient color value at layout (location = 112) -- type: vec3 */
		AmbientColor,

		/*! Emissive color value at layout (location = 113) -- type: vec3 */
		EmissiveColor,

		/*! Transparent color value at layout (location = 114) -- type: vec3 */
		TransparentColor,

		/*! Opacity value at layout (location = 115) -- type: float */
		Opacity,

		/*! Shininess factor at layout (location = 116) -- type: float */
		Shininess,

		/*! Shininess strength at layout (location = 117) -- type: float */
		ShininessStrength,

		/*! Refraction Index at layout (location = 118) -- type: float */
		RefractionIndex,

		/*! Reflectivity value at layout (location = 119) -- type: float */
		Reflectivity,

		/*! Diffuse texture sampler at layout (location = 120) -- type: sampler2D */
		DiffuseTexture,

		/*! Specular texture sampler at layout (location = 121) -- type: sampler2D */
		SpecularTexture,

		/*! Ambient texture sampler at layout (location = 122) -- type: sampler2D */
		AmbientTexture,

		/*! Emissive texture sampler at layout (location = 123) -- type: sampler2D */
		EmissiveTexture,

		/*! Height texture sampler at layout (location = 124) -- type: sampler2D */
		HeightTexture,

		/*! Normals texture sampler at layout (location = 125) -- type: sampler2D */
		NormalsTexture,

		/*! Shininess texture sampler at layout (location = 126) -- type: sampler2D */
		ShininessTexture,

		/*! Opacity texture sampler at layout (location = 127) -- type: sampler2D */
		OpacityTexture,

		/*! Displacement texture sampler at layout (location = 128) -- type: sampler2D */
		DisplacementTexture,

		/*! Reflection texture sampler at layout (location = 129) -- type: sampler2D */
		ReflectionTexture,

		/*! Lightmap texture sampler at layout (location = 130) -- type: sampler2D */
		LightmapTexture,

		/*! Albedo color at layout (location = 131) -- type: vec3 */
		Albedo,

		/*! Metallic factor at layout (location = 132) -- type: float */
		Metallic,

		/*! Smoothness factor at layout (location = 133) -- type: float */
		Smoothness,
		
		/*! Sheen factor at layout (location = 134) -- type: float */
		Sheen,

		/*! Thickness factor at layout (location = 135) -- type: float */
		Thickness,

		/*! Roughness factor at layout (location = 136) -- type: float */
		Roughness,

		/*! Anisotropy at layout (location = 137) -- type: float */
		Anisotropy,

		/*! Anisotropy Rotation at layout (location = 138) -- type: vec3 */
		AnisotropyRotation,

		/*! (Texturecoordinate-)Offset at layout (location = 139) -- type: vec2 */
		Offset,

		/*! (Texture-)Tiling or Texture Scale at layout (location = 140) -- type: vec2 */
		Tiling,
	};

	template<typename T>
	bool is_valid_MaterialUniformLocation(T value)
	{
		switch (static_cast<MaterialUniformLocation>(value))
		{
		case MaterialUniformLocation::DiffuseColor:
		case MaterialUniformLocation::SpecularColor:
		case MaterialUniformLocation::AmbientColor:
		case MaterialUniformLocation::EmissiveColor:
		case MaterialUniformLocation::TransparentColor:
		case MaterialUniformLocation::Opacity:
		case MaterialUniformLocation::Shininess:
		case MaterialUniformLocation::ShininessStrength:
		case MaterialUniformLocation::RefractionIndex:
		case MaterialUniformLocation::Reflectivity:
		case MaterialUniformLocation::DiffuseTexture:
		case MaterialUniformLocation::SpecularTexture:
		case MaterialUniformLocation::AmbientTexture:
		case MaterialUniformLocation::EmissiveTexture:
		case MaterialUniformLocation::HeightTexture:
		case MaterialUniformLocation::NormalsTexture:
		case MaterialUniformLocation::ShininessTexture:
		case MaterialUniformLocation::OpacityTexture:
		case MaterialUniformLocation::DisplacementTexture:
		case MaterialUniformLocation::ReflectionTexture:
		case MaterialUniformLocation::LightmapTexture:
		case MaterialUniformLocation::Albedo:
		case MaterialUniformLocation::Metallic:
		case MaterialUniformLocation::Smoothness:
		case MaterialUniformLocation::Sheen:
		case MaterialUniformLocation::Thickness:
		case MaterialUniformLocation::Roughness:
		case MaterialUniformLocation::Anisotropy:
		case MaterialUniformLocation::AnisotropyRotation:
		case MaterialUniformLocation::Offset:
		case MaterialUniformLocation::Tiling:
			return true;
		}
		return false;
	}
}