#version 330 core
out vec3 color;

in vec2 UV;

in vec3 esVertexPosition;
in vec3 esEyeDirection;
in vec3 esLightDirection;
in vec3 esNormal;
in vec3 halfVector;

uniform sampler2D myTextureSampler;

uniform vec3 materialAmbient;
uniform vec3 materialDiffuse;
uniform vec3 materialSpecular;
uniform float materialSpecExp;

uniform vec3 globalAmbient;
uniform vec3 lightDiffuse;
uniform vec3 lightSpecular;

void main()
{
    vec3 L = normalize(esLightDirection);
    vec3 N = normalize(esNormal);
    vec3 V = normalize(-esVertexPosition);
    vec3 H = normalize(halfVector);
    
    float cosPhi = dot(H,N);
    float cosTheta = dot(L,N);
    
    vec3 texColor = texture( myTextureSampler, UV ).rgb;
    
    vec3 ambient = (globalAmbient * materialAmbient).xyz;
    vec3 diffuse = lightDiffuse.xyz * materialDiffuse * max(cosTheta, 0.0);
    vec3 specular = lightSpecular.xyz * materialSpecular.xyz * pow(max(cosPhi,0.0), materialSpecExp);
    
    color = (ambient + diffuse) * texColor + specular;
}
