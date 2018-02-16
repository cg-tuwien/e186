#extension GL_ARB_explicit_uniform_location : enable

// ################# UNIFORM DATA ###############
uniform mat4 pvmtMatrix;
uniform mat4 vmtMmatrix;
uniform mat4 vMatrix;
uniform mat3 vmtNormalMatrix;

uniform vec2 uTexCoordsScale = vec2(1, 1);
// ----------------------------------------------

// ################### INPUT DATA ###############
layout (location = 0) in vec4 aVertexPosition; 
layout (location = 1) in vec2 aVertexTexCoord;
layout (location = 2) in vec3 aVertexNormal;
// ----------------------------------------------

// ################## OUTPUT DATA ###############
out VertexData
{
	vec2 texCoords;
	vec3 positionVS; ///< vertex position in view-space
	vec3 normalVS;   ///< vertex normal in view-space
} v_out;
// ----------------------------------------------

// ############ VERTEX SHADER MAIN ##############
void main()
{	
	v_out.texCoords = aVertexTexCoord * uTexCoordsScale;
	v_out.positionVS = (vmtMmatrix * aVertexPosition).xyz;
	v_out.normalVS = vmtNormalMatrix * aVertexNormal;

	gl_Position = pvmtMatrix * aVertexPosition;
}
// ----------------------------------------------
