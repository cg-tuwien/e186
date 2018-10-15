layout (location = 0) in vec4 aVertexPosition;
layout (location = 1) in vec2 aVertexTexCoord;
layout (location = 2) in vec3 aVertexNormal;

layout(location = 140) uniform vec2 uTexCoordsScale = vec2(1, 1);

out VertexData
{
    vec2 texCoords;
    vec3 positionMS; // vertex position in model space
    vec3 normalMS; // vertex normal in model space
} v_out;

void main()
{
    vec4 position = aVertexPosition;

    v_out.texCoords = aVertexTexCoord * uTexCoordsScale;
    v_out.positionMS = aVertexPosition.xyz;
    v_out.normalMS = aVertexNormal;

    gl_Position = vec4(aVertexPosition.xyz, 1);
}

