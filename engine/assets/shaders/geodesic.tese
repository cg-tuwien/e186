#version 410 core

layout(triangles, equal_spacing, cw) in;

uniform mat4 uProjection;
uniform mat4 uModelview;

in TescData
{
	vec3 tcPosition;
} tc_in[];

out TeseData
{
	vec3 tePosition;
	vec3 tePatchDistance;
} te_out;

void main()
{
	vec3 p0 = gl_TessCoord.x * tc_in[0].tcPosition;
	vec3 p1 = gl_TessCoord.y * tc_in[1].tcPosition;
	vec3 p2 = gl_TessCoord.z * tc_in[2].tcPosition;
	te_out.tePatchDistance = gl_TessCoord;
	te_out.tePosition = normalize(p0 + p1 + p2);
	gl_Position = uProjection * uModelview * vec4(te_out.tePosition, 1);
}
