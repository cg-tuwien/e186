#version 400

uniform mat4 mtMatrix;
uniform mat4 vMatrix;
uniform mat4 pMatrix;
uniform mat3 normalMatrix;

// -------------- Uniforms -----------------
uniform vec3 ambientLightColor;
uniform float shininessCoeff;
uniform float terrHeightMin;
uniform float terrHeightMax;

// directional light
uniform vec3 lightDirection;		///< light-direction in view-space, must be normalized

uniform sampler2D sampler;

// ---------------- varyings ---------------------
in Data
{
	vec3 varPositionWS; ///< vertex position in world-space
	vec3 varNormalWS;   ///< vertex normal in view-space
};

// ------------- output-color of fragment ------------
out vec4 fragColor;

void main()
{
	vec3 eyePosWS = mat3(inverse(transpose(vMatrix))) * vec3(0.0, 0.0, 0.0); // ????
	vec3 toEyeDir = normalize(eyePosWS - varPositionWS);
	vec3 normal = normalize(varNormalWS);

	// initialize all the colors
	vec3 ambient = ambientLightColor;
	vec3 diffuse = vec3(0, 0, 0);
	vec3 specular = vec3(0, 0, 0);

	vec3 toLightDirNrm = -lightDirection;
	vec3 halfVector = normalize(toLightDirNrm + toEyeDir);
	float NdotL = max(0.0, dot(toLightDirNrm, normal)); // lambertian coefficient
	float NdotH = max(0.0, dot(halfVector, normal));
	float specularPower = pow(NdotH, shininessCoeff);
		
	float sampleHeight = (varPositionWS.y - terrHeightMin) / (terrHeightMax - terrHeightMin);
	vec3 texCol = texture(sampler, vec2(sampleHeight, 0.5)).xyz;

	vec3 lightDiffCol = vec3(1, 1, 1);
	vec3 lightSpecCol = vec3(1, 1, 1);
	diffuse  += (lightDiffCol * NdotL * texCol);
	specular += (lightSpecCol * specularPower * texCol);

	// add all together
	fragColor = vec4(ambient + diffuse + specular, 1.0);
}