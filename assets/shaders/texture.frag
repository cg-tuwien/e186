#version 400

// -------------- uniforms -----------------
uniform sampler2DMS sampler;
uniform vec4 transform_mul = vec4(1, 1, 1, 1);
uniform vec4 transform_add = vec4(0, 0, 0, 0);

// ---------------- varyings ---------------------
in vec2 varTexCoords;

// ------------- output-color of fragment ------------
out vec4 fragColor;

void main()
{
	vec4 texCol = texelFetch(sampler, ivec2(varTexCoords * textureSize(sampler)), 0);
	fragColor = texCol * transform_mul + transform_add;
}