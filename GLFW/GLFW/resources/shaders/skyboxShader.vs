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

out vec3 textureCoords;
out vec3 fragmentNormal;
out vec3 fragmentPosition;

uniform QMaterial material;
uniform mat4 projectionMatrix, viewMatrix, modelMatrix;

void main()
{
    vec4 position = projectionMatrix * viewMatrix * modelMatrix * vec4(vPosition, 1.0f);
    gl_Position = position.xyww;

    fragmentNormal = mat3(transpose(inverse(modelMatrix))) * vNormal;
    fragmentPosition = vec3(modelMatrix * vec4(vPosition, 1.0f));

    textureCoords = vPosition;
}