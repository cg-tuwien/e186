#version 410 core

uniform mat3 uNormalMatrix;
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in TeseData
{
	vec3 tePosition;
	vec3 tePatchDistance;
} te_in[];

out GeomData
{
	vec3 gFacetNormal;
	vec3 gPatchDistance;
	vec3 gTriDistance;
};

void main()
{
	vec3 A = te_in[2].tePosition - te_in[0].tePosition;
	vec3 B = te_in[1].tePosition - te_in[0].tePosition;
	gFacetNormal = uNormalMatrix * normalize(cross(A, B));
    
	gPatchDistance = te_in[0].tePatchDistance;
	gTriDistance = vec3(1, 0, 0);
	gl_Position = gl_in[0].gl_Position; EmitVertex();

	gPatchDistance = te_in[1].tePatchDistance;
	gTriDistance = vec3(0, 1, 0);
	gl_Position = gl_in[1].gl_Position; EmitVertex();

	gPatchDistance = te_in[2].tePatchDistance;
	gTriDistance = vec3(0, 0, 1);
	gl_Position = gl_in[2].gl_Position; EmitVertex();

	EndPrimitive();
}