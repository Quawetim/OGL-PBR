#version 330 core

struct material
{
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

in vec3 FragmentPosition;
in vec2 UV;
in vec3 TangentLightPos;
in vec3 TangentViewPos;
in vec3 TangentFragPos;

out vec3 Color;

uniform vec3 LightColor;
uniform float LightConstant;
uniform float LightLinear;
uniform float LightQuadratic;
uniform float MaterialShine;
uniform bool Blinn;
uniform sampler2D DiffuseTexture;
uniform sampler2D NormalTexture;
uniform sampler2D SpecularTexture;
uniform material Material;
uniform pointLight PointLight[1];

void main()
{
	float GammaCorrection = 1.0;

	vec3 Normal = texture(NormalTexture, UV).rgb;
	Normal = normalize(Normal * 2.0 - 1.0);

	vec3 LightDirection = normalize(TangentLightPos - TangentFragPos);
	float diff = max(dot(LightDirection, Normal), 0.0f);

	vec3 ViewDirection = normalize(TangentViewPos - TangentFragPos);
	
	float spec;
	if (Blinn)
	{
		vec3 HalfwayDirection = normalize(LightDirection + ViewDirection);  
		spec = pow(max(dot(Normal, HalfwayDirection), 0.0), Material.Shine);
	}
	else
	{
		vec3 ReflectionDirection = reflect(-LightDirection, Normal);
		spec = pow(max(dot(ViewDirection, ReflectionDirection), 0.0f), Material.Shine);
	} 

	vec3 Ambient = 0.05f * texture(DiffuseTexture, UV).rgb;
	vec3 Diffuse = PointLight[0].Color * PointLight[0].Power * texture(DiffuseTexture, UV).rgb * diff;
	vec3 Specular = PointLight[0].Color * PointLight[0].Power * texture(SpecularTexture, UV).rgb * spec;

	float Distance = length(PointLight[0].Position - FragmentPosition);
	float Attenuation = 1.0f / (PointLight[0].Constant + PointLight[0].Linear * Distance + PointLight[0].Quadratic * (Distance * Distance));

	Ambient *= Attenuation;
	Diffuse *= Attenuation;
	Specular *= Attenuation;

	Color = Ambient + Diffuse + Specular;

	Color = pow(Color, vec3(1.0/GammaCorrection));
}