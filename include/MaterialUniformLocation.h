#pragma once
namespace e186
{
	/*! Vertex Attribute locations
	*/
	enum class MaterialUniformLocation : unsigned int
	{
		/*! Diffuse color at layout (location = 110) */
		DiffuseColor = 110,

		/*! Specular color at layout (location = 111) */
		SpecularColor,

		/*! Ambient color value at layout (location = 112) */
		AmbientColor,

		/*! Emissive color value at layout (location = 113) */
		EmissiveColor,

		/*! Transparent color value at layout (location = 114) */
		TransparentColor,

		/*! Opacity value at layout (location = 115) */
		Opacity,

		/*! Transparent color value at layout (location = 116) */
		Shininess,

		/*! Transparent color value at layout (location = 117) */
		ShininessStrength,

		/*! Transparent color value at layout (location = 118) */
		RefractionIndex,

		/*! Transparent color value at layout (location = 119) */
		Reflectivity,

		/*! Diffuse texture sampler at layout (location = 120) */
		DiffuseTexture,

		/*! Specular texture sampler at layout (location = 121) */
		SpecularTexture,

		/*! Ambient texture sampler at layout (location = 122) */
		AmbientTexture,

		/*! Emissive texture sampler at layout (location = 123) */
		EmissiveTexture,

		/*! Height texture sampler at layout (location = 124) */
		HeightTexture,

		/*! Normals texture sampler at layout (location = 125) */
		NormalsTexture,

		/*! Shininess texture sampler at layout (location = 126) */
		ShininessTexture,

		/*! Opacity texture sampler at layout (location = 127) */
		OpacityTexture,

		/*! Displacement texture sampler at layout (location = 128) */
		DisplacementTexture,

		/*! Reflection texture sampler at layout (location = 129) */
		ReflectionTexture,

		/*! Lightmap texture sampler at layout (location = 130) */
		LightmapTexture,

		/*! Albedo color at layout (location = 131) */
		Albedo,

		/*! Metallic factor at layout (location = 132) */
		Metallic,

		/*! Smoothness factor at layout (location = 133) */
		Smoothness,
		
		/*! Sheen factor at layout (location = 134) */
		Sheen,

		/*! Thickness factor at layout (location = 135) */
		Thickness,

		/*! Roughness factor at layout (location = 136) */
		Roughness,

		/*! Anisotropy at layout (location = 137) */
		Anisotropy,

		/*! Anisotropy Rotation at layout (location = 138) */
		AnisotropyRotation,

		/*! (Texturecoordinate-)Offset at layout (location = 139) */
		Offset,

		/*! (Texture-)Tiling or Texture Scale at layout (location = 140) */
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