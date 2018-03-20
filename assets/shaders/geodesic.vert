#version 410 core

layout (location = 0) in vec4 aPosition;
layout (location = 1) in vec4 a;
layout (location = 2) in vec4 b;
layout (location = 3) in vec4 c;
layout (location = 4) in vec4 d;
layout (location = 5) in vec4 e;
layout (location = 6) in vec4 f;

out VertData
{
	vec3 vPosition;
};

void main()
{
	vPosition = (aPosition + a + b + c + d + e + f).xyz;
}
