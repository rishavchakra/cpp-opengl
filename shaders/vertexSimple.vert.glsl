#version 330 core

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_color;

out vec3 vert_color;

void main()
{
    gl_Position = vec4(in_position.xyz, 1.0);
    // Passing the vertex color through to the fragment shader
    vert_color = in_color; 
}