#version 430

layout (location = 0) in vec3 aPos;

out vec3 textureCoords;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

void main()
{
    textureCoords = aPos;  
    gl_Position =  projectionMatrix * viewMatrix * vec4(aPos, 1.0);
}