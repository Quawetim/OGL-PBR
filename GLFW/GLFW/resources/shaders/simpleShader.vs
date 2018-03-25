#version 430 core

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vColor;

out vec3 vertexColor;

uniform mat4 projectionMatrix, viewMatrix, modelMatrix;

void main()
{	    
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vPosition, 1.0f);
    vertexColor = vColor;
}