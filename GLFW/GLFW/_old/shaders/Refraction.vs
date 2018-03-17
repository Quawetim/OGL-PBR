#version 330 core
layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;

out vec3 FragmentNormal;
out vec3 FragmentPosition;

uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

void main()
{
    gl_Position = P * V * M * vec4(VertexPosition, 1.0f);

    FragmentNormal = mat3(transpose(inverse(M))) * VertexNormal;

    FragmentPosition = vec3(M * vec4(VertexPosition, 1.0f));
}

