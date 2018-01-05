#version 440 core
#extension GL_ARB_explicit_uniform_location : enable

// -------------- uniforms -----------------
uniform vec3 uLightDirection;		///< light-direction in view-space, must be normalized
uniform vec3 uAmbientLightColor;

// Material data
layout(location = 10) uniform vec3 uDiffuseColor;
layout(location = 11) uniform vec3 uSpecularColor;
layout(location = 12) uniform vec3 uAmbientColor;
layout(location = 13) uniform vec3 uEmissiveColor;
layout(location = 16) uniform float uShininess;
layout(location = 20) uniform sampler2D uDiffuseTexSampler;
layout(location = 25) uniform sampler2D uNormalTexSampler;

// ---------------- varyings ---------------------
in vec2 vTexCoords;
in vec3 vPositionVS; ///< vertex position in view-space
in vec3 vNormalVS;   ///< vertex normal in view-space

// ------------- output-color of fragment ------------
out vec4 oFragColor;

void main()
{
	vec3 eyePosVS = vec3(0.0, 0.0, 0.0);
	vec3 toEyeDir = normalize(eyePosVS - vPositionVS);
	vec3 normal = normalize(vNormalVS);
	vec3 texCol = texture(uDiffuseTexSampler, vTexCoords).xyz;
	vec3 sampledNormal = texture(uNormalTexSampler, vTexCoords).xyz;

	// initialize all the colors
	vec3 lightAmbientCol = uAmbientLightColor;
	vec3 ambient = lightAmbientCol * uAmbientColor;
	vec3 emissive = uEmissiveColor;

	vec3 toLightDirNrm = -uLightDirection;
	vec3 halfVector = normalize(toLightDirNrm + toEyeDir);
	float NdotL = max(0.0, dot(toLightDirNrm, normal)); // lambertian coefficient
	float NdotH = max(0.0, dot(halfVector, normal));
	float specularPower = pow(NdotH, uShininess);
		
	vec3 lightDiffCol = vec3(1, 1, 1);
	vec3 lightSpecCol = vec3(1, 1, 1);
	vec3 diffuse  = (lightDiffCol * NdotL * (uDiffuseColor + sampledNormal) * texCol);
	vec3 specular = (lightSpecCol * specularPower * uSpecularColor * texCol);

	// add all together
	oFragColor = vec4(ambient + emissive + diffuse + specular, 1.0);
}