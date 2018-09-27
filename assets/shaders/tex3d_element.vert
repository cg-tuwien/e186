#version 430 core
#extension GL_ARB_explicit_uniform_location : enable

uniform mat4 pvmMatrix = mat4(1.0);

// TODO: threshold for not rendering

uniform vec3 uElementPos;
uniform vec3 uElementOffset = vec3(0, 0, 0);

layout (location = 0) in vec4 aVertexPosition; 
layout (location = 1) in vec3 aVertexNormal; 

out vec3 vertexNormal;

mat4 translationMatrix(vec3 tvec)
{    
	return mat4(1.0, 0.0, 0.0,  tvec.x,
				0.0, 1.0, 0.0,  tvec.y,
				0.0, 0.0, 1.0,  tvec.z,
				0.0, 0.0, 0.0,  1.0);
}

void main()
{	
	gl_Position = pvmMatrix * translationMatrix(uElementPos + uElementOffset) * vec4(0, 0, 0, 1);
}
