#version 330 core

layout(location = 0) in vec2 VertexPosition;
layout(location = 1) in vec2 VertexUV;

out vec2 UV;

void main()
{
	// Output position of the vertex, in clip space
	// map [0..800][0..600] to [-1..1][-1..1]
	vec2 NewVertexPosition = VertexPosition - vec2(400, 300); // [0..800][0..600] -> [-400..400][-300..300]
	NewVertexPosition /= vec2(400, 300);
	gl_Position = vec4(NewVertexPosition, 0.0f, 1.0f);
	
	// UV of the vertex.
	UV = VertexUV;
}

