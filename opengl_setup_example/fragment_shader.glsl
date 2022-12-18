#version 330 core
out vec3 color;
// in vec4 v_color;

in vec2 UV;

uniform sampler2D myTextureSampler;

void main()
{
  // color = v_color.rgb;
    color = texture( myTextureSampler, UV ).rgb;
}
