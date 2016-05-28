#version 330 core

layout (location = 0) in vec2 VertexPosition;

out vec2 TextureCoords;

uniform mat4 M;

void main()
{
	gl_Position = M * vec4(VertexPosition, 0.0f, 1.0f);
	TextureCoords = vec2((VertexPosition.x + 1.0f) / 2.0f, 1.0f - (VertexPosition.y + 1.0f) / 2.0f);
}

