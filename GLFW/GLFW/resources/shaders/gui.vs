#version 430 core

layout (location = 0) in vec2 vPosition;
layout (location = 1) in vec2 vTextureCoords;

out VS_OUT
{
    vec2 textureCoords;
} vs_out;

uniform mat4 projectionMatrix, modelMatrix;

void main()
{	    
	gl_Position = projectionMatrix * modelMatrix * vec4(vPosition, 0.0f, 1.0f);
    //vs_out.textureCoords = vec2((vPosition.x + 1.0f) / 2.0f, 1.0f + (vPosition.y + 1.0f) / 2.0f);
    vs_out.textureCoords = vTextureCoords;
}