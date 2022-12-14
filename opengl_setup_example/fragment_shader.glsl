#version 330 core
out vec3 color;
in vec4 v_color;
void main()
{
  color = v_color.rgb;
}
