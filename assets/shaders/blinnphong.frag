#extension GL_ARB_explicit_uniform_location : enable

// ################# UNIFORM DATA ###############
layout(location = 140) uniform vec2 uTexCoordsScale = vec2(1, 1);

// Material data:
layout(location = 110) uniform vec3 uDiffuseReflectivity;
layout(location = 111) uniform vec3 uSpecularReflectivity;
layout(location = 112) uniform vec3 uAmbientReflectivity;
layout(location = 113) uniform vec3 uEmissiveLight;
layout(location = 116) uniform float uShininess;
layout(location = 120) uniform sampler2D uDiffuseTexSampler;
// ----------------------------------------------

// #################### LIGHTS ##################
struct AmbientLightData
{
	vec4 color;
};

struct DirectionalLightData
{
	vec4 direction;
	vec4 color;
};

struct PointLightData
{
	vec4 position;
	vec4 color;
	vec4 attenuation;
};

uniform AmbientLightData uAmbientLight;
uniform DirectionalLightData uDirectionalLight;
uniform PointLightData uPointLight;
// ----------------------------------------------

// ################# VARYING DATA ###############
in VertexData
{
	vec2 texCoords;
	vec3 positionVS; ///< vertex position in view-space
	vec3 normalVS;   ///< vertex normal in view-space
} fs_in;
// ----------------------------------------------

// ################## OUTPUT DATA ###############
out vec4 oFragColor;
// ----------------------------------------------

// ############### HELPER FUNCTIONS #############
// Calculates the light attenuation divident for the given attenuation vector.
// @param atten attenuation data
// @param dist  distance
// @param dist2 squared distance
float CalcAttenuation(vec4 atten, float dist, float dist2)
{
	return atten[0] + atten[1] * dist + atten[2] * dist2;
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
	vec3 pos_vs = fs_in.positionVS;
	vec3 eye_pos_vs = vec3(0.0, 0.0, 0.0);
	vec3 to_eye_nrm_vs = normalize(eye_pos_vs - pos_vs);
	vec3 normal_vs = normalize(fs_in.normalVS);
	vec2 scaledTexCoords = uTexCoordsScale * fs_in.texCoords;
	vec3 diff_tex_color = texture(uDiffuseTexSampler, scaledTexCoords).rgb;
	
	// initialize all the colors
	vec3 ambient = uAmbientLight.color.rgb * uAmbientReflectivity * diff_tex_color;
	vec3 emissive = uEmissiveLight;
	vec3 diffuse_and_specular  = vec3(0,0,0);

	// directional light
	vec3 to_light_dir_vs = -uDirectionalLight.direction.xyz;
	vec3 dl_intensity = uDirectionalLight.color.rgb;
	diffuse_and_specular += dl_intensity * CalcBlinnPhongDiffAndSpecContribution(to_light_dir_vs, to_eye_nrm_vs, normal_vs, diff_tex_color);
	
	// point light 1
	vec3 light_pos_vs = uPointLight.position.xyz;
	vec3 to_light = light_pos_vs - pos_vs;
	float dist_sq = dot(to_light, to_light);
	float dist = sqrt(dist_sq);
	vec3 to_light_nrm = to_light / dist;
	float atten = CalcAttenuation(uPointLight.attenuation, dist, dist_sq);
	vec3 pl_intensity = uPointLight.color.rgb / atten;
	diffuse_and_specular += pl_intensity * CalcBlinnPhongDiffAndSpecContribution(to_light_nrm, to_eye_nrm_vs, normal_vs, diff_tex_color);

	// add all together
	oFragColor = vec4(ambient + emissive + diffuse_and_specular, 1.0);
}
// ----------------------------------------------
