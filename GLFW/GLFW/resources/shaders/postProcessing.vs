#version 430

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec2 vTextureCoords;

out VS_OUT
{
    vec2 textureCoords;
} vs_out;

void main()
{
    gl_Position = vec4(vPosition.x, vPosition.y, 0.0f, 1.0f);
       
    vs_out.textureCoords = vTextureCoords;
} 