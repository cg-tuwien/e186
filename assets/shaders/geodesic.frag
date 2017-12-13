#version 410 core

uniform vec3 uLightPosition;
uniform vec3 uDiffuseMaterial;
uniform vec3 uAmbientMaterial;

in GeomData
{
	vec3 gFacetNormal;
	vec3 gPatchDistance;
	vec3 gTriDistance;
};

out vec4 oFragColor;

float amplify(float d, float scale, float offset)
{
	d = scale * d + offset;
	d = clamp(d, 0, 1);
	d = 1 - exp2(-2*d*d);
	return d;
}

void main()
{
	vec3 N = normalize(gFacetNormal);
	vec3 L = uLightPosition;
	float df = abs(dot(N, L));
	vec3 color = uAmbientMaterial + df * uDiffuseMaterial;

	float d1 = min(min(gTriDistance.x, gTriDistance.y), gTriDistance.z);
	float d2 = min(min(gPatchDistance.x, gPatchDistance.y), gPatchDistance.z);
	color = amplify(d1, 40, -0.5) * amplify(d2, 60, -0.5) * color;

	oFragColor = vec4(color, 1.0);
}
