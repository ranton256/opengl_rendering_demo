#version 330 core
layout (location = 0) in vec3 a_position;
layout(location = 1) in vec3 vertexNormal;

out vec3 eyeSpaceNormal;
out vec3 eyeVertexPosition;
out vec3 lightDirection;

uniform mat4 mvMatrix;
uniform mat4 projMatrix;

void main( )
{
    gl_Position = projMatrix * mvMatrix * vec4(a_position, 1.0);
    UV = vertexUV;
}


