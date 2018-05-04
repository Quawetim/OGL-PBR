#version 430 core

in VS_OUT
{
    vec3 vertexColor;
} fs_in;

out vec3 fragmentColor;

void main()
{	
    fragmentColor = fs_in.vertexColor;
}