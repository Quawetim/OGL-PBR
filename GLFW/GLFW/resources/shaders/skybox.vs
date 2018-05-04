#version 430

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTextureCoords;
layout (location = 3) in vec3 vTangent;
layout (location = 4) in vec3 vBitangent;

out VS_OUT
{
    vec3 textureCoords;
} vs_out;

uniform mat4 projectionMatrix, viewMatrix, modelMatrix;

void main()
{
    vec4 position = projectionMatrix * viewMatrix * modelMatrix * vec4(vPosition, 1.0f);
    gl_Position = position.xyww;

    vs_out.textureCoords = vPosition;
}