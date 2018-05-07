#version 430 core

in VS_OUT
{
    vec2 textureCoords;
} fs_in;

out vec3 fragmentColor;

uniform sampler2D bgTexture;

void main()
{	
    fragmentColor = texture(bgTexture, fs_in.textureCoords).rgb;
}