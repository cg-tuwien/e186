#extension GL_ARB_explicit_uniform_location : enable

// ################# UNIFORM DATA ###############

// Material data:
layout(location = 110) uniform vec3 uDiffuseReflectivity;
layout(location = 111) uniform vec3 uSpecularReflectivity;
layout(location = 112) uniform vec3 uAmbientReflectivity;
layout(location = 113) uniform vec3 uEmissiveLight;
layout(location = 116) uniform float uShininess;
layout(location = 120) uniform sampler2D uDiffuseTexSampler;
layout(location = 125) uniform sampler2D uNormalTexSampler;

uniform float uNormalMappingStrength = 1.0;
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

// ################### INPUT DATA ###############
in VertexData
{
	vec3 toEyeDirTS;			///< direction from vertex towards the eye in tangent space
	vec2 texCoords;				///< texture coordinates
	vec3 directionalLightDirTS;	///< directional light's direction in tangent space
	vec3 pointLightDirTS;		///< point light towards vertex direction in tangent space
	vec3 positionVS;			///< interpolated vertex position in view-space
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
// @param dist3 cubed distance
float CalcAttenuation(vec4 atten, float dist, float dist2, float dist3)
{
	return atten[0] + atten[1] * dist + atten[2] * dist2 + atten[3] * dist3;
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
	
	vec3 diffuse = uDiffuseReflectivity * diffuse_reflectivity_from_tex * n_dot_l; // component-wise product
	vec3 specular = uSpecularReflectivity * spec_pwr;
	return diffuse + specular;
}
// ----------------------------------------------

// ############ FRAGMENT SHADER MAIN ############

// Calculates the normalized normal in tangent space by sampling the
// normal from the normal map (also mix between the normal map's normal
// and the vertex' normal is performed here)
vec3 CalcNormalizedNormalTS()
{
	// sample the normal from the normal map => it is already in tangent space
	vec3 normalSmpl = normalize(texture(uNormalTexSampler, fs_in.texCoords).rgb * 2.0 - 1.0);
	vec3 normalTS = mix(vec3(0,0,1), normalSmpl, clamp(uNormalMappingStrength, 0.0, 1.0));
	return normalize(normalTS);
}

// Calculates the diffuse and specular illumination contribution for all the light sources.
// All calculations are performed in tangent space
vec3 CalculateDiffuseAndSpecularIlluminationInTS(vec3 diff_tex_color)
{
	vec3 diffuse_and_specular = vec3(0, 0, 0);

	vec3 to_eye_nrm_ts = normalize(fs_in.toEyeDirTS);

	// Get the normal (samples from normal map, also consideres the normal-mapping-strength param)
	vec3 normal_ts = CalcNormalizedNormalTS();
	
	// directional light
	vec3 to_dir_light_ts = -normalize(fs_in.directionalLightDirTS);
	vec3 dl_intensity = uDirectionalLight.color.rgb;
	diffuse_and_specular += dl_intensity * CalcBlinnPhongDiffAndSpecContribution(to_dir_light_ts, to_eye_nrm_ts, normal_ts, diff_tex_color);
	
	// calculate for a single point light

	vec3 to_pt_light_ts = -normalize(fs_in.pointLightDirTS);
	float dist = distance(fs_in.positionVS, uPointLight.position.xyz);
	float atten = CalcAttenuation(uPointLight.attenuation, dist, dist * dist, dist * dist * dist);
	vec3 pl_intensity = uPointLight.color.rgb / atten;
	diffuse_and_specular += pl_intensity * CalcBlinnPhongDiffAndSpecContribution(to_pt_light_ts, to_eye_nrm_ts, normal_ts, diff_tex_color);

	return diffuse_and_specular;
}

void main()
{
	// Sample the diffuse color 
	vec3 diff_tex_color = texture(uDiffuseTexSampler, fs_in.texCoords).rgb;
	// initialize all the colors
	vec3 ambient = uAmbientLight.color.rgb * uAmbientReflectivity * diff_tex_color;
	vec3 emissive = uEmissiveLight;
	vec3 diffuse_and_specular = CalculateDiffuseAndSpecularIlluminationInTS(diff_tex_color);
	
	// add all together
	oFragColor = vec4(ambient + emissive + diffuse_and_specular, 1.0);
}
// ----------------------------------------------
