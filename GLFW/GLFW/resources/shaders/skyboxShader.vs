#version 430

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTextureCoords;
layout (location = 3) in vec3 vTangent;
layout (location = 4) in vec3 vBitangent;

struct QMaterial
{
    vec3 ambientColor;
    vec3 diffuseColor;
    vec3 specularColor;
    float shininess;
};

out VS_OUT
{
    vec3 textureCoords;
    vec3 fragmentNormal;
    vec3 fragmentPosition;
} vs_out;

uniform QMaterial material;
uniform mat4 projectionMatrix, viewMatrix, modelMatrix;

void main()
{
    vec4 position = projectionMatrix * viewMatrix * modelMatrix * vec4(vPosition, 1.0f);
    gl_Position = position.xyww;

    vs_out.fragmentNormal = mat3(transpose(inverse(modelMatrix))) * vNormal;
    vs_out.fragmentPosition = vec3(modelMatrix * vec4(vPosition, 1.0f));

    vs_out.textureCoords = vPosition;
}