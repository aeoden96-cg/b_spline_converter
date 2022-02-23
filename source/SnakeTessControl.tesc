#version 430 core


in float vRadius[ ];
in vec3 vCenter[ ];
in float tessColor[];

patch out float tcRadius;
patch out vec3 tcCenter;
patch out float tcColor;

//uniform float uDetail;
//uniform float uScale;

layout (vertices = 1)  out;

void main( )
{

    gl_out[gl_InvocationID].gl_Position = gl_in[0].gl_Position;

    tcCenter = vCenter[0];
    tcRadius = vRadius[0];
    tcColor= tessColor[0];

    gl_TessLevelOuter[0] = 8.;
    gl_TessLevelOuter[1] = 8;//uScale * tcRadius * uDetail;
    gl_TessLevelOuter[2] = 8.;
    gl_TessLevelOuter[3] = 8;//uScale * tcRadius * uDetail;

    gl_TessLevelInner[0]  = 1;//uScale * tcRadius * uDetail;
    gl_TessLevelInner[1]  = 1;//uScale * tcRadius * uDetail;

}

