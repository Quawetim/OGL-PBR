#version 330 core

layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec3 VertexColor;

out vec3 FragmentColor;

uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

void main()
{	
	gl_Position =  P * V * M * vec4(VertexPosition, 1.0f);
	FragmentColor = VertexColor;
}

