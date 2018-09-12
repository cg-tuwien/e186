// -------------- uniforms -----------------
#if defined(MULTISAMPLE)
uniform sampler2DMS sampler;
#elif defined(SHADOW)
uniform sampler2DShadow sampler;
#else
uniform sampler2D sampler;
#endif
uniform vec4 transform_mul = vec4(1, 1, 1, 1);
uniform vec4 transform_add = vec4(0, 0, 0, 0);

// ---------------- varyings ---------------------
in vec2 varTexCoords;

// ------------- output-color of fragment ------------
layout (location = 0) out vec4 fragColor;

void main()
{
#if defined(MULTISAMPLE)
	vec4 texCol = texelFetch(sampler, ivec2(varTexCoords * textureSize(sampler)), 0);
	fragColor = texCol * transform_mul + transform_add;
#elif defined(SHADOW)
	float s = texture(sampler, vec3(varTexCoords, 0.99));
	vec4 texCol = vec4(s, s, s, 1.0);
	fragColor = texCol;
#else
	vec4 texCol = texelFetch(sampler, ivec2(varTexCoords * textureSize(sampler, 0)), 0);
	fragColor = texCol * transform_mul + transform_add;
#endif
}