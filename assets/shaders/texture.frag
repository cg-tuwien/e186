#version 400

// -------------- uniforms -----------------
uniform sampler2D sampler;
uniform vec4 color;

// ---------------- varyings ---------------------
in vec2 varTexCoords;

// ------------- output-color of fragment ------------
out vec4 fragColor;

void main()
{
	vec3 texCol = texture(sampler, varTexCoords).xyz;
    fragColor = vec4(texCol, 1.0) * color;
}