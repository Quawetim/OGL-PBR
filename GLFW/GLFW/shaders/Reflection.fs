#version 330 core

in vec3 FragmentNormal;
in vec3 FragmentPosition;

out vec4 Color;

uniform samplerCube SkyBox;
uniform vec3 CameraPosition;

void main()
{      
	vec3 I = normalize(FragmentPosition - CameraPosition);
	vec3 R = reflect(I, normalize(FragmentNormal));  
    
	Color = texture(SkyBox, R);
}