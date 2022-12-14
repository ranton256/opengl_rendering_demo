#version 330 core
layout ( location = 0 ) in vec3 a_position;
out vec4 v_color;
uniform vec4 tri_color;

uniform mat4 mvp_matrix;
// uniform mat4 u_proj_matrix;
// uniform mat4 u_model_matrix;
void main( )
{
    v_color = tri_color;
    // mat4 mvp_matrix = u_proj_matrix * u_model_matrix;
    gl_Position = mvp_matrix * vec4(a_position, 1.0);
}


