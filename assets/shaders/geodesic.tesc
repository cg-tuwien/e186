#version 410 core

uniform float uInnerTessLevel;
uniform float uOuterTessLevel;

layout(vertices = 3) out;

in VertData
{
	vec3 vPosition;
} v_in[];

out TescData
{
	vec3 tcPosition;
} tc_out[];

#define ID gl_InvocationID

void main()
{
	tc_out[ID].tcPosition = v_in[ID].vPosition.xyz;
	gl_TessLevelInner[0]  = uInnerTessLevel;
	gl_TessLevelOuter[ID] = uOuterTessLevel;
}