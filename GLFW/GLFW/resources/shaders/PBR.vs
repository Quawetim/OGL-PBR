#version 430

#define MAX_LIGHTS 5;

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTextureCoords;
layout (location = 3) in vec3 vTangent;
layout (location = 4) in vec3 vBitangent;

struct Light
{
    vec3 position;
    float radius;

    vec3 diffuseColor;
    vec3 specularColor;
    float power;
};

out VS_OUT
{
    vec3 fragmentPosition;
    vec3 fragmentNormal;
    vec2 textureCoords;
    vec3 cameraPosition; 
} vs_out;

uniform mat4 projectionMatrix, viewMatrix, modelMatrix;

uniform vec3 cameraPosition;

uniform int lightsCount;
uniform Light light[5];

bool useNormalMaps;

void main()
{
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vPosition, 1.0f);
    
    vs_out.fragmentPosition = vec3(modelMatrix * vec4(vPosition, 1.0f));
    vs_out.fragmentNormal = mat3(transpose(inverse(modelMatrix))) * vNormal;  
    vs_out.textureCoords = vTextureCoords;

    vs_out.cameraPosition = cameraPosition;
} 