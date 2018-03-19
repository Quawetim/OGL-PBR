#version 430

in vec2 textureCoords;

out vec4 color;

struct QMaterial
{
    vec3 ambientColor;
    vec3 diffuseColor;
    vec3 specularColor;
    float shinePower;
};

uniform QMaterial material;

uniform bool diffuseMap1_flag;
uniform bool specularMap1_flag;
uniform bool normalMap1_flag;

uniform sampler2D diffuseMap1;
uniform sampler2D specularMap1;
uniform sampler2D normalMap1;

void main()
{
    if (diffuseMap1_flag)
    {
        color = texture(diffuseMap1, textureCoords);
    }
    else
    {
        color = vec4(material.diffuseColor, 1.0f);
    } 
}