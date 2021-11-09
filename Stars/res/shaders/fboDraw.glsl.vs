#version 330 core

layout(location = 0) in vec4 in_position;

out vec2 texCoords;
void main()
{
    texCoords = in_position.zw;
    gl_Position =  vec4(in_position.xy, 0, 1.0);
}