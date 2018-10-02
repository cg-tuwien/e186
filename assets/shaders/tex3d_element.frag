#version 430 core
#extension GL_ARB_explicit_uniform_location : enable

uniform vec4 uColor;

// TODO: directional light position and lambertian shading!

in vec3 vertexNormal;
in vec3 vertexColor;

// ------------- output-color of fragment ------------
layout (location=0) out vec4 oFragColor;

void main()
{
	oFragColor = vec4(vertexColor, 1.0);
}
