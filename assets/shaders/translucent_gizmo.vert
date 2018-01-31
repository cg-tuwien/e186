#version 430 core
#extension GL_ARB_explicit_uniform_location : enable

uniform mat4 pvmtMatrix;

layout (location = 0) in vec4 aVertexPosition; 

void main()
{	
	gl_Position = pvmtMatrix * aVertexPosition;
}
