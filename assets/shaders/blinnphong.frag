#version 440 core
#extension GL_ARB_explicit_uniform_location : enable

// ################# UNIFORM DATA ###############
uniform mat4 pvmtMatrix;
uniform mat4 vmtMmatrix;
uniform mat4 vMatrix;
uniform mat3 vmtNormalMatrix;

uniform vec3 uAmbientIllumination;

// Material data:
layout(location = 10) uniform vec3 uDiffuseReflectivity;
layout(location = 11) uniform vec3 uSpecularReflectivity;
layout(location = 12) uniform vec3 uAmbientReflectivity;
layout(location = 13) uniform vec3 uEmissiveLight;
layout(location = 16) uniform float uShininess;
layout(location = 20) uniform sampler2D uDiffuseTexSampler;
// ----------------------------------------------

// ################## SSBO DATA #################
// Match the structure in the C++ code
// I don't use vec3 because padding
// rules sometimes differ between c++
// and OpenGL
struct PointLightData
{
  vec4 position;
  vec4 color;
  vec4 atten;
};

// force this block to be assigned to index 100
// std140 specifies that the structured data should
// follow a specific set of packing rules. This will
// make the packing consistent across OpenGL implementations
layout(std430, binding = 0) buffer PointLightBlock
{
    PointLightData uPointLights[];
};
// ----------------------------------------------

// ################# VARYING DATA ###############
in vec2 vTexCoords;
in vec3 vPositionVS; ///< vertex position in view-space
in vec3 vNormalVS;   ///< vertex normal in view-space
// ----------------------------------------------

// ################## OUTPUT DATA ###############
out vec4 oFragColor;
// ----------------------------------------------

// ############### HELPER FUNCTIONS #############
// Calculates the light attenuation divident for the given light source.
// @param dist  distance
// @param dist2 squared distance
// @param dist3 cubed distance
float CalcAttenuation(int lightIdx, float dist, float dist2, float dist3)
{
	return    uPointLights[lightIdx].atten[0]
			+ uPointLights[lightIdx].atten[1] * dist
			+ uPointLights[lightIdx].atten[2] * dist2
			+ uPointLights[lightIdx].atten[3] * dist3;
}

// Calculates the diffuse and specular illumination contribution for the given
// parameters according to the Blinn-Phong lighting model.
// All parameters must be normalized.
vec3 CalcBlinnPhongDiffAndSpecContribution(vec3 to_light, vec3 to_eye, vec3 normal, vec3 diffuse_reflectivity_from_tex)
{
	float n_dot_l = max(0.0, dot(normal, to_light)); // lambertian coefficient
	vec3 half_vec = normalize(to_light + to_eye);
	float n_dot_h = max(0.0, dot(normal, half_vec));
	float spec_pwr = pow(n_dot_h, uShininess);
	
	vec3 diffuse = uDiffuseReflectivity * diffuse_reflectivity_from_tex * n_dot_l;
	vec3 specular = uSpecularReflectivity * spec_pwr;
	return diffuse + specular;
}
// ----------------------------------------------

void main()
{
	vec3 eye_pos_vs = vec3(0.0, 0.0, 0.0);
	vec3 to_eye_nrm = normalize(eye_pos_vs - vPositionVS);
	vec3 normal = normalize(vNormalVS);
	vec3 diff_tex_color = texture(uDiffuseTexSampler, vTexCoords).xyz;
	
	// initialize all the colors
	vec3 ambient = uAmbientIllumination * uAmbientReflectivity;
	vec3 emissive = uEmissiveLight;
	vec3 diffuse_and_specular  = vec3(0,0,0);

	for (int i = 0; i < uPointLights.length(); ++i)
	{
		vec3 light_pos_vs = (vMatrix * vec4(uPointLights[i].position.xyz, 1)).xyz;
		vec3 to_light = light_pos_vs - vPositionVS;
		float dist_sq = dot(to_light, to_light);
		float dist = sqrt(dist_sq);
		vec3 to_light_nrm = to_light / dist;

		float atten = CalcAttenuation(i, dist, dist_sq, dist * dist_sq);
		vec3 light_intensity = uPointLights[i].color.rgb / atten;
		
		diffuse_and_specular += light_intensity * CalcBlinnPhongDiffAndSpecContribution(to_light_nrm, to_eye_nrm, normal, diff_tex_color);
	}

	// add all together
	oFragColor = vec4(ambient + emissive + diffuse_and_specular, 1.0);
}