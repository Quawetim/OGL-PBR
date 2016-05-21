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

in vec3 FragmentNormal;
in vec3 FragmentPosition;

out vec3 Color;

uniform bool Blinn;
uniform material Material;
uniform vec3 CameraPosition;
uniform int LightsCount;
uniform pointLight PointLight[MAX_POINT_LIGHTS];

vec3 ComputePointLight(int id, vec3 normal, vec3 fragpos, vec3 viewdir)
{
	vec3 position = PointLight[id].Position;
	vec3 LightDirection = normalize(position - fragpos);

	float diff = max(dot(normal, LightDirection), 0.0f);

	float spec;
	if (Blinn)
	{
		vec3 HalfwayDirection = normalize(LightDirection + viewdir);  
        spec = pow(max(dot(normal, HalfwayDirection), 0.0), Material.Shine);
	}
	else
	{
		vec3 ReflectionDirection = reflect(-LightDirection, normal);
		spec = pow(max(dot(viewdir, ReflectionDirection), 0.0f), Material.Shine);
	}
	float Distance = length(position - fragpos);

	float Attenuation = 1.0f / (PointLight[id].Constant + PointLight[id].Linear * Distance + PointLight[id].Quadratic * (Distance * Distance));
	
	vec3 Ambient = Material.AmbientColor * Material.DiffuseColor;
	vec3 Diffuse = PointLight[id].Color * PointLight[id].Power * Material.DiffuseColor * diff;
	vec3 Specular = PointLight[id].Color * PointLight[id].Power * Material.SpecularColor * spec;

	Ambient *= Attenuation;
	Diffuse *= Attenuation;
	Specular *= Attenuation;
	
	return (Ambient + Diffuse + Specular);
}

void main()
{	
	float GammaCorrection = 1.0;

	if (LightsCount > 0)
	{
		vec3 Normal = normalize(FragmentNormal);
		vec3 ViewDirection = normalize(CameraPosition - FragmentPosition);

		for (int i = 0; i < LightsCount && i <= MAX_POINT_LIGHTS; i++) Color += ComputePointLight(i, Normal, FragmentPosition, ViewDirection);		
	}
	else
	{
		Color = Material.DiffuseColor;
	}

	Color = pow(Color, vec3(1.0/GammaCorrection));
}