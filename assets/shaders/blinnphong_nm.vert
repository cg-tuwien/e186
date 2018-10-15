#extension GL_ARB_explicit_uniform_location : enable

// ################# UNIFORM DATA ###############
uniform mat4 vmMatrix;
uniform mat4 pMatrix;

layout(location = 140) uniform vec2 uTexCoordsScale = vec2(1, 1);
// ----------------------------------------------

// ################### INPUT DATA ###############
layout (location = 0) in vec4 aPosition; 
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aNormal;
layout (location = 8) in vec3 aTangent;
layout (location = 9) in vec3 aBitangent;
// ----------------------------------------------

// ################## OUTPUT DATA ###############
out VertexData
{
	vec3 toEyeDirTS;
	vec2 texCoords;
	vec3 directionalLightDirTS;
	vec3 pointLightDirTS;
	vec3 positionVS;

} v_out;
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

// ############### HELPER FUNCTIONS #############
// Re-orthogonalizes the first vector w.r.t. the second vector (Gram-Schmidt process)
vec3 ReOrthogonalize(vec3 first, vec3 second)
{
	return normalize(first - dot(first, second) * second);
}
// ----------------------------------------------

// ############ VERTEX SHADER MAIN ##############
void main()
{
	vec4 positionOS = aPosition;
	vec4 positionVS = vmMatrix * positionOS;
	vec4 positionCS = pMatrix * positionVS;
	
	// perform shading in tangent space => build the TBN matrix to transform all the stuff:
	vec3 normalOS     = normalize(aNormal);
    vec3 tangentOS    = ReOrthogonalize(aTangent, normalOS);
    //vec3 bitangentOS  = ReOrthogonalize(aBitangent, normalOS);
	vec3 bitangentOS = cross(normalOS, tangentOS);
	// if texture is mirrored, flip bitangent
	if (dot(bitangentOS, aBitangent) < 0)
		bitangentOS = -bitangentOS;
	mat3 matrixTStoOS = mat3(tangentOS, bitangentOS, normalOS);
	mat3 matrixOStoTS = transpose(matrixTStoOS);
	mat3 matrixVStoTS = matrixOStoTS * mat3(inverse(vmMatrix));

	// transform all the stuff into tangent space and pass to the fragment shader:
	
	// Calculate direction from vertex towards eye:
	v_out.toEyeDirTS = matrixVStoTS * normalize(vec3(0, 0, 0) - positionVS.xyz);
	
	// Pass the texture coordinates:
	v_out.texCoords = aTexCoords * uTexCoordsScale;
	
	// Transform the directional light's direction:
	vec3 dir_light_direction_vs = uDirectionalLight.direction.xyz;
	v_out.directionalLightDirTS = matrixVStoTS * dir_light_direction_vs;

	// Calculate the light direction from the point light towards the vertex, also calculate the distance:
	vec3 point_light_dir_vs = positionVS.xyz - uPointLight.position.xyz;
	v_out.pointLightDirTS = matrixVStoTS * point_light_dir_vs;
	v_out.positionVS = positionVS.xyz;

	gl_Position = positionCS;
}
