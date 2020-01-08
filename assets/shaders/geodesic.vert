#version 410 core

layout (location = 0) in vec4 aVertexPosition; 

out VertData
{
	vec3 vPosition;
};

void main()
{
	vPosition = aVertexPosition.xyz;
}
