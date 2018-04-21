#version 430 core

out vec3 fragmentColor;

uniform vec3 color;

void main()
{	
    fragmentColor = color;
}