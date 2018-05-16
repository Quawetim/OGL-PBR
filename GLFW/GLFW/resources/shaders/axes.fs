#version 430 core

in VS_OUT
{
    vec3 vertexColor;
} fs_in;

out vec4 fragmentColor;

void main()
{	
    fragmentColor = vec4(fs_in.vertexColor, 1.0f);
}