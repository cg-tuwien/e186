layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out; // options are points, line_strip, triangle_strip. we want output triangles.

uniform mat4 uViewProjMatOrthoX; // look at origin from +X, +Y is up, orthographic proj to unit cube
uniform mat4 uViewProjMatOrthoY; // look at origin from +Y, -Z is up, orthographic proj to unit cube
uniform mat4 uViewProjMatOrthoZ; // look at origin from +Z, +Y is up, orthographic proj to unit cube

uniform int uVoxelGridResolution; // used for x, y, z

in VertexData
{
    vec2 texCoords;
    vec3 positionMS; // vertex position in model space
    vec3 normalMS;   // vertex normal in model space
} v_in[];

out VertexData
{
    vec2 texCoords;
    vec3 positionMS;
    vec3 normalMS;
} v_out;

flat out int axisOfTriangleProjection; // to restore 3D voxel data in fragment shader, flat to avoid interpolation

// we want to use the GPU 2D rasterizer to help in voxelizing our triangles,
// the rasterizer would produce a raster of fragments in the view direction
// but we want a raster in the directions of an axis aligned 3D voxel grid
// so before the rasterization, we use geometry shader to project triangles
// along axis that maximizes the area of the triangle 2D projection, to minimize information loss
// after rasterization, the fragment shader computes 3D voxel data from all triangle fragments in the voxel

void main() {

    mat4 projMat;

    // determine in which coordinate axis the triangle normal is facing the most
    // to find the projection axis that yields the maximum projected area
    vec3 N = normalize(v_in[0].normalMS);
    if (abs(N.x) > abs(N.y) && abs(N.x) > abs(N.z))
    {
        axisOfTriangleProjection = 0;
        projMat = uViewProjMatOrthoX;
    }
    else if (abs(N.y) > abs(N.x) && abs(N.y) > abs(N.z))
    {
        axisOfTriangleProjection = 1;
        projMat = uViewProjMatOrthoY;
    }
    else
    {
        axisOfTriangleProjection = 2;
        projMat = uViewProjMatOrthoZ;
    }

    // project triangle vertices to normalized device coordinates
    // using orthographic projection (no perspective divide)

    vec4 positions[3];
    positions[0] = projMat * gl_in[0].gl_Position;
    positions[1] = projMat * gl_in[1].gl_Position;
    positions[2] = projMat * gl_in[2].gl_Position;

    // store vertex data

    for (int i = 0; i < 3; ++i)
    {
        gl_Position = positions[i];
        v_out.positionMS = positions[i].xyz;
        v_out.normalMS = v_in[i].normalMS;
        v_out.texCoords = v_in[i].texCoords;
        EmitVertex();
    }

    EndPrimitive();

}
