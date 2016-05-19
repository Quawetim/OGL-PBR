#version 330 core

#define MAX_POINT_LIGHTS 10

struct material
{
	vec3 AmbientColor;
	vec3 DiffuseColor;
	vec3 SpecularColor;
	float Shine;
};

struct pointLight
{
	vec3 Position;
	vec3 Color;
	float Power;
	float Constant;
	float Linear;
	float Quadratic;
};

in vec3 Normal;
in vec3 FragmentPosition;

out vec3 Color;

uniform material Material;
uniform vec3 CameraPosition;
uniform int LightsCount;
uniform pointLight PointLight[MAX_POINT_LIGHTS];

vec3 ComputePointLight(vec3 position, vec3 normal, vec3 fragpos, vec3 viewdir)
{
	vec3 LightDirection = normalize(position - fragpos);

	float diff = max(dot(normal, LightDirection), 0.0f);
	vec3 ReflectionDirection = reflect(-LightDirection, normal);

	float spec = pow(max(dot(viewdir, ReflectionDirection), 0.0f), Material.Shine);
	float Distance = length(position - fragpos);

	float Attenuation = 1.0f / (1.0f + 0.09f * Distance + 0.032f * (Distance * Distance));
	
	vec3 Ambient = Material.AmbientColor * Material.DiffuseColor;
	vec3 Diffuse =  Material.DiffuseColor * diff;
	vec3 Specular = Material.SpecularColor * spec;

	Ambient *= Attenuation;
	Diffuse *= Attenuation;
	Specular *= Attenuation;
	
	return (Ambient + Diffuse + Specular);
}

void main()
{	
	if (LightsCount > 0)
	{
		vec3 Norm = normalize(Normal);
		vec3 ViewDirection = normalize(CameraPosition - FragmentPosition);

		for (int i = 0; i < LightsCount && i <= MAX_POINT_LIGHTS; i++) Color += ComputePointLight(PointLight[i].Position, Norm, FragmentPosition, ViewDirection);
	}
	else
	{
		Color = Material.DiffuseColor;
	}
}