#version 430

in VS_OUT
{
    vec3 textureCoords;
} fs_in;

out vec3 fragmentColor;

uniform samplerCube envMap;

void main()
{
    fragmentColor = texture(envMap, fs_in.textureCoords).rgb;
}