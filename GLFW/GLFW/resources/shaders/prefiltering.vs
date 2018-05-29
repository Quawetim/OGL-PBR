#version 430

layout (location = 0) in vec3 vPosition;

out vec3 textureCoords;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

void main()
{     
    gl_Position = projectionMatrix * viewMatrix * vec4(vPosition, 1.0f);
    textureCoords = vPosition;
}