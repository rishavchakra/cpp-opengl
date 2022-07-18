#version 330 core

uniform vec4 uni_color;
in vec3 vert_color;
out vec4 out_color;

void main()
{
    // out_color = vec4(0.8f, 0.3f, 0.4f, 1.0f);
    // out_color = uni_color;
    out_color = vec4(vert_color.xyz, 1.0f);
}