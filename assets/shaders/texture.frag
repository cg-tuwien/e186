#version 400

// -------------- uniforms -----------------
uniform sampler2D sampler;
uniform vec4 transform_mul = vec4(1, 1, 1, 1);
uniform vec4 transform_add = vec4(0, 0, 0, 0);

// ---------------- varyings ---------------------
in vec2 varTexCoords;

// ------------- output-color of fragment ------------
out vec4 fragColor;

void main()
{
	vec4 texCol = texture(sampler, varTexCoords);
    fragColor = texCol * transform_mul + transform_add;
}