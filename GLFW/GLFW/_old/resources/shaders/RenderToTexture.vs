#version 330 core

layout(location = 0) in vec3 vertexPosition;

out vec2 UV;

uniform mat4 MVP;

void main()
{		
	//gl_Position = vec4(vertexPosition, 1.0);
	gl_Position = MVP * vec4(vertexPosition, 1.0);

	UV = (vertexPosition.xy + vec2(1.0, 1.0)) / 2.0;
}

