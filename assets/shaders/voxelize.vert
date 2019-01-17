layout (location = 0) in vec4 aVertexPosition;
layout (location = 1) in vec2 aVertexTexCoord;

uniform float uScaleFactor; // scale to fit model vertices into viewport (viewport is set to voxel grid size)
layout(location = 140) uniform vec2 uTexCoordsScale = vec2(1, 1);

out VertexData
{
    vec2 texCoords;
} v_out;

void main()
{
    v_out.texCoords = aVertexTexCoord * uTexCoordsScale;
    gl_Position = vec4(aVertexPosition.xyz * uScaleFactor, 1);
}

