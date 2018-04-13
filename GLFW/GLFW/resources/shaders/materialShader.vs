#version 430

#define MAX_LIGHTS 5;

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTextureCoords;
layout (location = 3) in vec3 vTangent;
layout (location = 4) in vec3 vBitangent;

struct Material
{
    vec3 ambientColor;
    vec3 diffuseColor;
    vec3 specularColor;
    float shininess;
};

struct Light
{
    vec3 position;
    float radius;

    vec3 diffuseColor;
    vec3 specularColor;
    float power;
};

out VS_OUT
{
    vec3 fragmentPosition;
    vec3 fragmentNormal;
    vec2 textureCoords;

    vec3 cameraPosition;

    Light light;
} vs_out;

uniform Material material;
uniform mat4 projectionMatrix, viewMatrix, modelMatrix;

uniform vec3 cameraPosition;

uniform bool useNormalMaps;

void main()
{
    bool useGramSchmidt = true;

    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vPosition, 1.0f);
    
    vs_out.fragmentPosition = vec3(modelMatrix * vec4(vPosition, 1.0f));
    vs_out.fragmentNormal = mat3(transpose(inverse(modelMatrix))) * vNormal;    
    vs_out.textureCoords = vTextureCoords;

    vs_out.cameraPosition = cameraPosition;

    vs_out.light.position = vec3(0.0f, 0.0f, 15.0f);
    vs_out.light.radius = 1.0f;

    vs_out.light.diffuseColor = vec3(0.8f, 0.8f, 0.92f);
    vs_out.light.specularColor = vec3(0.7f, 0.7f, 0.7f);
    vs_out.light.power = 460.0f;

    if (useNormalMaps)
    {
        vec3 normal = normalize(vec3(modelMatrix * vec4(vNormal, 0.0f)));
        vec3 tangent = normalize(vec3(modelMatrix * vec4(vTangent, 0.0f)));
        
        vec3 bitangent;

        if (useGramSchmidt)
        {
            tangent = normalize(tangent - dot(tangent, normal) * normal);
            bitangent = cross(normal, tangent);
        }
        else
        {
            bitangent = normalize(vec3(modelMatrix * vec4(vBitangent, 0.0f)));
        }

        mat3 TBN = transpose(mat3(tangent, bitangent, normal));

        vs_out.fragmentPosition = TBN * vs_out.fragmentPosition;
        vs_out.cameraPosition = TBN * cameraPosition;

        vs_out.light.position = TBN * vs_out.light.position;
    }
} 