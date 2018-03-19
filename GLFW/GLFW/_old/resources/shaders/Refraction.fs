#version 330 core

in vec3 FragmentNormal;
in vec3 FragmentPosition;

out vec4 Color;

uniform samplerCube SkyBox;
uniform vec3 CameraPosition;
uniform float RefractiveIndex;

void main()
{          
    vec3 I = normalize(FragmentPosition - CameraPosition);
    vec3 R = refract(I, normalize(FragmentNormal), 1.0f / RefractiveIndex);
    Color = texture(SkyBox, R);
}