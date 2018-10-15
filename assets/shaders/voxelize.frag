// output voxel data 3D textures using image load/store interface
// TODO use other formats depending on data, rgba8 for color, rgba16f for normals
layout(binding = 0, rgba16f) uniform image3D uVoxelDiffuseReflectivity;
layout(binding = 1, rgba16f) uniform image3D uVoxelNormal;

// model data
layout(location = 140) uniform vec2 uTexCoordsScale = vec2(1, 1);
layout(location = 110) uniform vec3 uDiffuseReflectivity;
layout(location = 120) uniform sampler2D uDiffuseTexSampler;

uniform int uVoxelGridResolution; // used for x, y, z

in VertexData
{
    vec2 texCoords;
    vec3 positionMS;
    vec3 normalMS;
} v_in;

flat in int axisOfTriangleProjection; // to restore 3D voxel data, flat to avoid interpolation

// gl_FragCoord.xy are the window space coordinates as given by glViewport
// we use pixel_center_integer layout qualifier to have pixel centers at integer coords rather than default half integer
// so gl_FragCoord.xy are in [0, resolutionX - 1][0, resolutionY - 1]
// gl_FragCoord.z stores the depth in [0,1] between near and far plane,
// here depth is linear due to orthographic projection
layout(pixel_center_integer) in vec4 gl_FragCoord;

void main()
{
    ivec3 voxelCoord;

    // determine voxel coordinates
    // after the rasterization step
    // gl_FragCoord stores xy and depth coordinates in a local frame without relation to the world context
    // so depending on the lookAt matrix used before rasterization
    // the gl_FragCoord xyz may not map directly to the world XYZ coords.
    // when storing to a 2D image that doesnt matter, but we want an axis-aligned voxel grid,
    // so we need to remap according to the lookAt matrices used in geometry shader.
    // note: depth of the projected triangle is stored in gl_FragCoord.z without perspective nonlinear distortion
    float depth = gl_FragCoord.z;
    if (axisOfTriangleProjection == 0) // x
    {
        voxelCoord.x = int(uVoxelGridResolution - uVoxelGridResolution * depth);
        voxelCoord.y = int(gl_FragCoord.y);
        voxelCoord.z = int(gl_FragCoord.x);
    }
    else if (axisOfTriangleProjection == 1) // y
    {
        voxelCoord.x = int(gl_FragCoord.x);
        voxelCoord.y = int(uVoxelGridResolution - uVoxelGridResolution * depth);
        voxelCoord.z = int(gl_FragCoord.y);
    }
    else if (axisOfTriangleProjection == 2) // z
    {
        voxelCoord.x = int(gl_FragCoord.x);
        voxelCoord.y = int(gl_FragCoord.y);
        voxelCoord.z = int(uVoxelGridResolution * depth);
    }

    // store voxel data
    // TODO average values from all projected triangles that coincide on the voxel using atomic add

    vec2 scaledTexCoords = uTexCoordsScale * v_in.texCoords;
    vec3 diff_tex_color = texture(uDiffuseTexSampler, scaledTexCoords).rgb;

    imageStore(uVoxelDiffuseReflectivity, voxelCoord, vec4(diff_tex_color, 1.0));

    //imageStore(uVoxelNormal, voxelCoord, vec4(v_in.normalMS, 0.0));


}
