#version 330 core

in vec3 TexCoords;

out vec4 Color;

uniform samplerCube SkyBox;

void main()
{	
	Color = texture(SkyBox, TexCoords);
}