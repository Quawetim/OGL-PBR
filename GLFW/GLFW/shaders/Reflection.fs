#version 330 core
in vec3 Normal;
in vec3 Position;

out vec4 Color;

uniform vec3 CameraPosition;
uniform samplerCube SkyBox;

void main()
{             
    vec3 I = normalize(Position - CameraPosition);
    vec3 R = reflect(I, normalize(Normal));
    Color = texture(SkyBox, R);
}