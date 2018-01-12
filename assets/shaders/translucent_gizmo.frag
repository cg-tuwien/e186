#version 440 core
#extension GL_ARB_explicit_uniform_location : enable

uniform vec4 uColor;

// ------------- output-color of fragment ------------
out vec4 oFragColor;

void main()
{
	oFragColor = uColor;
}