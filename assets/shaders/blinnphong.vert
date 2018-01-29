#version 440 core
#extension GL_ARB_explicit_uniform_location : enable

uniform mat4 pvmtMatrix;
uniform mat4 vmtMmatrix;
uniform mat4 vMatrix;
uniform mat3 vmtNormalMatrix;

layout (location = 0) in vec4 aVertexPosition; 
layout (location = 1) in vec2 aVertexTexCoord;
layout (location = 2) in vec3 aVertexNormal;

out vec2 vTexCoords;
out vec3 vPositionVS;
out vec3 vNormalVS;

void main()
{	
	vTexCoords = aVertexTexCoord;
	vPositionVS = (vmtMmatrix * aVertexPosition).xyz;
	vNormalVS = vmtNormalMatrix * aVertexNormal;

	gl_Position = pvmtMatrix * aVertexPosition;
}
