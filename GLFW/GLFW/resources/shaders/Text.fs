#version 330 core

in vec2 UV;

out vec4 Color;

uniform sampler2D Texture;
uniform vec3 TextColor;

void main()
{	
	Color = texture(Texture, UV) * vec4(TextColor, 1.0f);
}