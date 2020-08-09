#version 330 core
layout (location=0) in vec3 position;
layout (location = 1) in vec3 color;

varying vec2 transform_to_frag;
out vec4 vertex_color;

void main()
{
    gl_Position = vec4(position, 1.0);
    vertex_color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}
