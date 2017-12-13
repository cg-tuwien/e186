#version 400

uniform mat4 mtMatrix;
uniform mat4 vMatrix;
uniform mat4 pMatrix;
uniform mat3 normalMatrix;

layout (location = 0) in vec4 vertexPosition; 
layout (location = 2) in vec3 vertexNormal;

out Data 
{
	vec3 varPositionWS;
	vec3 varNormalWS;
};

void main()
{	
	vec4 vertexWS = mtMatrix * vertexPosition;
	vec4 vertexVS = vMatrix * vertexWS;
	vec4 vertexCS = pMatrix * vertexVS;

	varPositionWS = vertexWS.xyz;
	varNormalWS = normalMatrix * vertexNormal;

	gl_Position = vertexCS;
}
