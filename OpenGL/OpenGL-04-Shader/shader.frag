#version 330 core
in vec4 vertex_color;

varying vec2 transform_to_frag;
uniform sampler2D color_map;

out vec4 color;

void main()
{
    color = vertex_color;
    gl_FragColor = texture2D(color_map, transform_to_frag);
}
