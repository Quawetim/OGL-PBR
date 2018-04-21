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

    vec3 lightsPositions[5];
} vs_out;

uniform Material material;
uniform mat4 projectionMatrix, viewMatrix, modelMatrix;

uniform vec3 cameraPosition;

uniform bool useNormalMaps;

uniform int lightsCount;
uniform Light lights[5];

void main()
{
    bool useGramSchmidt = true;

    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vPosition, 1.0f);
    
    vs_out.fragmentPosition = vec3(modelMatrix * vec4(vPosition, 1.0f));
    vs_out.fragmentNormal = mat3(transpose(inverse(modelMatrix))) * vNormal;    
    vs_out.textureCoords = vTextureCoords;

    vs_out.cameraPosition = cameraPosition;

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

        for (int i = 0; i < lightsCount; i++)
        {
            vs_out.lightsPositions[i] = TBN * lights[i].position;
        }
    }
} 