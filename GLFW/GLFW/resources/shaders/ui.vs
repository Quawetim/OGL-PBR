#version 430 core

layout (location = 0) in vec2 vPosition;
layout (location = 1) in vec2 vTextureCoords;

out VS_OUT
{
    vec2 textureCoords;
} vs_out;

uniform mat4 projectionMatrix;

void main()
{	    
    gl_Position = projectionMatrix * vec4(vPosition, 0.0f, 1.0f);    
    vs_out.textureCoords = vTextureCoords;
}