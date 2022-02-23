#version 330 core

layout(location = 0) in vec4 vertexPosition_modelspace;
layout(location = 1) in float vertexColor;

uniform mat4 MVP;

// GOING TO Tessellation shader.
out float tessColor;

// GOING TO Tessellation shader.
out vec3 vCenter;
out float vRadius;


void main(){
    //We want to use MVP here, but we also want to keep w coordinate untouched (its a radius).
    vCenter = (MVP * vec4(vertexPosition_modelspace.xyz,1.0)).xyz ;
    vRadius = vertexPosition_modelspace.w;

    tessColor = vertexColor;

    //gl_Position = vertexPosition_modelspace - vec4(0.5,0.5,0,0);

}

