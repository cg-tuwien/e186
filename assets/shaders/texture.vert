uniform mat4 pvmtMatrix = mat4(1.0);

layout (location = 0) in vec4 vertexPosition;
layout (location = 1) in vec2 vertexTexCoord;

out vec2 varTexCoords;

void main()
{
	varTexCoords = vertexTexCoord;
	gl_Position = pvmtMatrix * vertexPosition;
}