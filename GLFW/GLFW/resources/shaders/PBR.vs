#version 430

#define MAX_MAPS 5

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTextureCoords;
layout (location = 3) in vec3 vTangent;
layout (location = 4) in vec3 vBitangent;

struct Material
{
    vec3 albedo;
    float metallic;
    float smoothness;
    float surfaceHeight;
};

out VS_OUT
{
    vec3 fragmentPosition;
    vec3 fragmentNormal;
    vec2 textureCoords;
    vec3 cameraPosition; 

    // Tangent Space
    vec3 fragmentPositionTBN;
    vec3 cameraPositionTBN;   
} vs_out;

uniform mat4 projectionMatrix, viewMatrix, modelMatrix;
uniform vec3 cameraPosition;

uniform Material material;

uniform bool useHeightMaps;
uniform int heightMapsCount;
uniform sampler2D heightMaps[MAX_MAPS];

void main()
{
    const bool useDisplacementMapping = true;
    vec3 vertexPosition = vPosition;

    if (useHeightMaps && useDisplacementMapping)
    {
        for (int i = 0; i < heightMapsCount && i <= MAX_MAPS; i++)
        {
            float height = texture(heightMaps[i], vTextureCoords).r;
            
            if (vNormal.x == 0.0f && vNormal.y == 1.0f && vNormal.z == 0.0f) height = 0.5f;
            if (vNormal.x == 0.0f && vNormal.y == -1.0f && vNormal.z == 0.0f) height = 0.5f;
            
            vertexPosition += vec3(vNormal * height * material.surfaceHeight);
        }
    }

    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertexPosition, 1.0f);
    
    vs_out.fragmentPosition = vec3(modelMatrix * vec4(vPosition, 1.0f));
    vs_out.fragmentNormal = mat3(transpose(inverse(modelMatrix))) * vNormal;  
    vs_out.textureCoords = vTextureCoords;

    vs_out.cameraPosition = cameraPosition;

    vec3 T = normalize(mat3(modelMatrix) * vTangent);
    vec3 B = normalize(mat3(modelMatrix) * vBitangent);
    vec3 N = normalize(mat3(modelMatrix) * vNormal);
    mat3 TBN = transpose(mat3(T, B, N));

    vs_out.cameraPositionTBN = TBN * cameraPosition;
    vs_out.fragmentPositionTBN = TBN * vs_out.fragmentPosition;
} 