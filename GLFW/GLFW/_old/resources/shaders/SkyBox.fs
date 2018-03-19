#version 330 core

#define MAX_POINT_LIGHTS 10

struct pointLight
{
	vec3 Position;
	vec3 Color;
	float Power;
	float Constant;
	float Linear;
	float Quadratic;
};

in vec3 FragmentNormal;
in vec3 FragmentPosition;
in vec3 TexCoords;

out vec4 Color;

uniform samplerCube SkyBox;
uniform vec3 CameraPosition;
uniform int LightsCount;
uniform pointLight PointLight[MAX_POINT_LIGHTS];

vec4 ComputePointLight(int id, vec3 normal, vec3 fragpos, vec3 viewdir)
{
	vec3 position = PointLight[id].Position;
	vec3 LightDirection = normalize(position - fragpos);

	float diff = max(dot(normal, LightDirection), 0.0f);
	vec3 ReflectionDirection = reflect(-LightDirection, normal);

	float Distance = length(position - fragpos);

	float Attenuation = 1.0f / (PointLight[id].Constant + PointLight[id].Linear * Distance + PointLight[id].Quadratic * (Distance * Distance));
	
	vec4 Ambient = vec4(0.05f, 0.05f, 0.05f, 1.0f) * texture(SkyBox, TexCoords);
	vec3 diffuse = PointLight[id].Color * PointLight[id].Power;
	vec4 Diffuse = vec4(diffuse, 1.0f) * texture(SkyBox, TexCoords) * diff;

	Ambient *= Attenuation;
	Diffuse *= Attenuation;
	
	return (Ambient + Diffuse);
}

void main()
{	
	if (LightsCount > 0)
	{
		vec3 Normal = normalize(FragmentNormal);
		vec3 ViewDirection = normalize(CameraPosition - FragmentPosition);

		for (int i = 0; i < LightsCount && i <= MAX_POINT_LIGHTS; i++) Color += ComputePointLight(i, Normal, FragmentPosition, ViewDirection);
	}
	else
	{
		Color = texture(SkyBox, TexCoords);
	}
}