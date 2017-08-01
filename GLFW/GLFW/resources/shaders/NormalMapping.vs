#version 330 core

struct pointLight
{
	vec3 Position;
	vec3 Color;
	float Power;
	float Constant;
	float Linear;
	float Quadratic;
};

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec2 VertexUV;
layout (location = 3) in vec3 VertexTangent;
layout (location = 4) in vec3 VertexBitangent;

out vec3 FragmentPosition;
out vec2 UV;
out vec3 TangentLightPos;
out vec3 TangentViewPos;
out vec3 TangentFragPos;

uniform mat4 P;
uniform mat4 V;
uniform mat4 M;
uniform vec3 CameraPosition;
uniform pointLight PointLight[1];

void main()
{	
	gl_Position =  P * V * M * vec4(VertexPosition, 1.0f);	
	FragmentPosition = vec3(M * vec4(VertexPosition, 1.0f));
	UV = VertexUV;

	mat3 NormalMatrix = transpose(inverse(mat3(M)));
	vec3 Tangent = normalize(NormalMatrix * VertexTangent);
	vec3 Bitangent = normalize(NormalMatrix * VertexBitangent);
	vec3 Normal = normalize(NormalMatrix * VertexNormal);

	mat3 TBN = transpose(mat3(Tangent, Bitangent, Normal));

	TangentLightPos = TBN * PointLight[0].Position;
	TangentViewPos = TBN * CameraPosition;
	TangentFragPos = TBN * FragmentPosition;		
}
