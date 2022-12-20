#version 330 core
layout (location = 0) in vec3 a_position;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec3 vertexNormal;

out vec2 UV;

out vec3 esVertexPosition;
out vec3 esEyeDirection;
out vec3 esLightDirection;
out vec3 esNormal;
out vec3 halfVector;

uniform vec3 lightPosition;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform mat4 normMatrix;

void main( )
{
    gl_Position = projMatrix * viewMatrix * modelMatrix * vec4(a_position, 1.0);
    UV = vertexUV;
    
    esVertexPosition = (viewMatrix * modelMatrix * vec4(a_position,1)).xyz;
    
    esEyeDirection = vec3(0,0,0) - esVertexPosition;

    vec3 esLightPosition = ( viewMatrix * vec4(lightPosition,1)).xyz;
    esLightDirection = esLightPosition - esVertexPosition;
    
    esNormal = (normMatrix * vec4(vertexNormal,0)).xyz;
    
    halfVector = (esLightPosition + (-esVertexPosition)).xyz;
}


