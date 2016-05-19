#version 330 core

layout(location = 0) in vec3 VertexPosition;

out vec3 TexCoords;

uniform mat4 P;
uniform mat4 V;

void main()
{		
	gl_Position =  (P * V * vec4(VertexPosition, 1.0f)).xyww;
	TexCoords = VertexPosition;
}

