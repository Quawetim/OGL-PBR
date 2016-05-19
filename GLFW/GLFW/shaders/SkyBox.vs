#version 330 core

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;

out vec3 FragmentNormal;
out vec3 FragmentPosition;
out vec3 TexCoords;

uniform mat4 P;
uniform mat4 V;

void main()
{		
	gl_Position =  (P * V * vec4(VertexPosition, 1.0f)).xyww;
	FragmentPosition = VertexPosition;
	FragmentNormal = VertexNormal;
	TexCoords = VertexPosition;
}

