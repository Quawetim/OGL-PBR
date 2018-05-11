#version 430 core

in VS_OUT
{
    vec2 textureCoords;
} fs_in;

out vec3 fragmentColor;

uniform vec3 bgColor;
uniform bool useBgTexture;
uniform sampler2D bgTexture;

void main()
{	
    if (useBgTexture)
    {
        fragmentColor = texture(bgTexture, fs_in.textureCoords).rgb;
    }
    else
    {
        fragmentColor = bgColor;
    }
}