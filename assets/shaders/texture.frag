// -------------- uniforms -----------------
#ifdef MULTISAMPLE
uniform sampler2DMS sampler;
#else
uniform sampler2D sampler;
#endif
uniform vec4 transform_mul = vec4(1, 1, 1, 1);
uniform vec4 transform_add = vec4(0, 0, 0, 0);

// ---------------- varyings ---------------------
in vec2 varTexCoords;

// ------------- output-color of fragment ------------
out vec4 fragColor;

void main()
{
#ifdef MULTISAMPLE
	vec4 texCol = texelFetch(sampler, ivec2(varTexCoords * textureSize(sampler)), 0);
#else
	vec4 texCol = texelFetch(sampler, ivec2(varTexCoords * textureSize(sampler, 0)), 0);
#endif
	fragColor = texCol * transform_mul + transform_add;
}