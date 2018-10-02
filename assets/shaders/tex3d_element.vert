#version 430 core
#extension GL_ARB_explicit_uniform_location : enable

uniform mat4 pvmMatrix = mat4(1.0);
uniform sampler3D uSampler;
uniform ivec3 uTexDimensions;
uniform vec3 uElementOffset = vec3(0, 0, 0);

layout (location = 0) in vec4 aVertexPosition; 
layout (location = 1) in vec3 aVertexNormal; 

out vec3 vertexNormal;
out vec3 vertexColor;

mat4 translationMatrix(vec3 tvec)
{    
	return mat4(1.0, 0.0, 0.0,  0.0,
				0.0, 1.0, 0.0,  0.0,
				0.0, 0.0, 1.0,  0.0,
				tvec.x, tvec.y, tvec.z,  1.0);
}

void main()
{	
	int slice_dim = uTexDimensions.x * uTexDimensions.y;
	int row_dim = uTexDimensions.x;

	int z = gl_InstanceID / slice_dim;
	int y = (gl_InstanceID - slice_dim * z) / row_dim;
	int x = gl_InstanceID - row_dim * y - slice_dim * z;

	vec3 tx = texelFetch(uSampler, ivec3(x, y, z), 0).xyz;
	vertexColor = tx;
	const vec3 empty_tx = vec3(0, 0, 0);
	if (empty_tx == tx)
	{
		gl_Position = vec4(0, 0, 0, 0);
	}
	else
	{
		gl_Position = pvmMatrix * translationMatrix(vec3(x, y, z) + uElementOffset) * aVertexPosition;
	}
}
