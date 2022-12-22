#version 330 core
out vec3 color;

in vec2 UV;

in vec3 esVertexPosition;
in vec3 esEyeDirection;
in vec3 esLightDirection;
in vec3 esNormal;
in vec3 halfVector;

uniform sampler2D myTextureSampler;

// diffuse color comes from the
// uniform vec3 materialAmbient;
// uniform vec3 materialSpecular
// uniform float materialSpecExp;

//uniform vec3 globalAmbient;
//uniform vec3 lightDiffuse;
//uniform vec3 lightSpecular

void main()
{
    // TODO: use uniforms for these.
    vec3 materialAmbient = vec3(0.1, 0.08, 0.11);
    vec3 materialDiffuse = vec3(0.8, 0.8, 0.8);
    vec3 materialSpecular = vec3(1,1,1);
    float materialSpecExp = 10;

    vec3 globalAmbient = vec3(1,1,1);
    vec3 lightDiffuse = vec3(1,1,1);
    vec3 lightSpecular = vec3(1,1,1);

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
