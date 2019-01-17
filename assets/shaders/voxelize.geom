layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out; // options are points, line_strip, triangle_strip. we want output triangles.

uniform mat4 uViewProjMatOrthoX; // look at origin from +X, +Y is up, orthographic proj to unit cube
uniform mat4 uViewProjMatOrthoY; // look at origin from +Y, -Z is up, orthographic proj to unit cube
uniform mat4 uViewProjMatOrthoZ; // look at origin from +Z, +Y is up, orthographic proj to unit cube

uniform int uVoxelGridResolution; // used for x, y, z

in VertexData
{
    vec2 texCoords;
} v_in[];

out VertexData
{
    vec3 faceNormal;
    vec2 texCoords;
} v_out;

flat out int axisOfTriangleProjection; // to restore 3D voxel data in fragment shader, flat to avoid interpolation

// we want to use the GPU 2D rasterizer to help in voxelizing our triangles,
// the rasterizer would produce a raster of fragments in the view direction
// but we want a raster in the directions of an axis aligned 3D voxel grid
// so before the rasterization, we use geometry shader to project triangles
// along axis that maximizes the area of the triangle 2D projection, to minimize information loss
// after rasterization, the fragment shader computes 3D voxel data from all triangle fragments in the voxel

void main() {

    vec4 pos[3]; // triangle verts
    pos[0] = gl_in[0].gl_Position;
    pos[1] = gl_in[1].gl_Position;
    pos[2] = gl_in[2].gl_Position;

    mat4 projMat;

    // determine which coordinate axis the triangle normal is facing most
    // to use as projection axis that yields maximum projected area
    vec3 N = cross(pos[1].xyz - pos[0].xyz, pos[2].xyz - pos[0].xyz);
    vec3 absN = abs(N);
    if (absN.x > absN.y && absN.x > absN.z)
    {
        axisOfTriangleProjection = 0;
        projMat = uViewProjMatOrthoX;
    }
    else if (absN.y > absN.x && absN.y > absN.z)
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
    // and store vertex data

    for (int i = 0; i < 3; ++i)
    {
        gl_Position = projMat * pos[i];
        v_out.faceNormal = mat3(projMat) * N;
        v_out.texCoords = v_in[i].texCoords;
        EmitVertex();
    }

    EndPrimitive();

}
