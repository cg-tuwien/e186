#pragma once
namespace e186
{
	/*! Vertex Attribute locations
	*/
	enum class MaterialUniformLocation : unsigned int
	{
		/*! Diffuse color at layout (location = 10) */
		DiffuseColor = 10,

		/*! Specular color at layout (location = 11) */
		SpecularColor,

		/*! Ambient color value at layout (location = 12) */
		AmbientColor,

		/*! Emissive color value at layout (location = 13) */
		EmissiveColor,

		/*! Transparent color value at layout (location = 14) */
		TransparentColor,

		/*! Opacity value at layout (location = 15) */
		Opacity,

		/*! Transparent color value at layout (location = 16) */
		Shininess,

		/*! Transparent color value at layout (location = 17) */
		ShininessStrength,

		/*! Transparent color value at layout (location = 18) */
		RefractionIndex,

		/*! Transparent color value at layout (location = 19) */
		Reflectivity,

		/*! Diffuse texture sampler at layout (location = 20) */
		DiffuseTexture,

		/*! Specular texture sampler at layout (location = 21) */
		SpecularTexture,

		/*! Ambient texture sampler at layout (location = 22) */
		AmbientTexture,

		/*! Emissive texture sampler at layout (location = 23) */
		EmissiveTexture,

		/*! Height texture sampler at layout (location = 24) */
		HeightTexture,

		/*! Normals texture sampler at layout (location = 25) */
		NormalsTexture,

		/*! Shininess texture sampler at layout (location = 26) */
		ShininessTexture,

		/*! Opacity texture sampler at layout (location = 27) */
		OpacityTexture,

		/*! Displacement texture sampler at layout (location = 28) */
		DisplacementTexture,

		/*! Reflection texture sampler at layout (location = 29) */
		ReflectionTexture,

		/*! Lightmap texture sampler at layout (location = 30) */
		LightmapTexture
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
			return true;
		}
		return false;
	}
}