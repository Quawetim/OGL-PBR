#version 330 core
in vec3 Normal;
in vec3 Position;
out vec4 color;

uniform vec3 CameraPosition;
uniform samplerCube SkyBox;
uniform float RefractiveIndex;

void main()
{          
    vec3 I = normalize(Position - CameraPosition);
    vec3 R = refract(I, normalize(Normal), 1.0f / RefractiveIndex);
    color = texture(SkyBox, R);
}