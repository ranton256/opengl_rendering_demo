#version 330 core
layout ( location = 0 ) in vec3 a_position;
out vec4 v_color;
uniform vec4 tri_color;

uniform mat4 mvp_matrix;

void main( )
{
    v_color =- vec4(a_position, 1.0) * 0.5 + vec4(0.5, 0.5, 0.5, 0.5) * tri_color;
    gl_Position = mvp_matrix * vec4(a_position, 1.0);
}


