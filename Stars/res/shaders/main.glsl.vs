#version 330 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_color;

out VertexData
{
    vec3 color;
} vertex;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform float u_time;
uniform vec4 u_color;
uniform bool u_isSun;

float rand(vec2 co){
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

void main()
{
    if(u_isSun)
    {
        vertex.color = in_color + vec3(5, 5, 5);
    }
    else
    {
        vertex.color = in_color;
    }
    gl_Position =  vec4(in_position, 1.0);
}