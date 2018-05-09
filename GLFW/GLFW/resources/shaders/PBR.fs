#version 430

#define MAX_MAPS 5
#define MAX_LIGHTS 5

struct Material
{
    vec3 ambientColor;
    vec3 diffuseColor;
    vec3 specularColor;
    float shininess;
    float refractiveIndex;
    float reflectiveIndex;

    float metallic;
    float roughness;    
};

struct Light
{
    vec3 position;
    float radius;

    vec3 diffuseColor;
    vec3 specularColor;
    float power;
};

in VS_OUT
{
    vec3 fragmentPosition;
    vec3 fragmentPositionTBN;

    vec3 fragmentNormal;
    vec2 textureCoords;

    vec3 cameraPosition; 
    vec3 cameraPositionTBN;

    vec3 lightsPositionsTBN[MAX_LIGHTS];
} fs_in;

out vec4 fragmentColor;

const float PI = 22.0f / 7.0f;

uniform Material material;

uniform bool useDiffuseMaps;
uniform int diffuseMapsCount;
uniform sampler2D diffuseMaps[MAX_MAPS];

uniform bool useSpecularMaps;
uniform int specularMapsCount;
uniform sampler2D specularMaps[MAX_MAPS];

uniform bool useNormalMaps;
uniform int normalMapsCount;
uniform sampler2D normalMaps[MAX_MAPS];

uniform int lightsCount;
uniform Light light[MAX_LIGHTS];

uniform samplerCube environmentMap;
uniform samplerCube irradianceMap;
uniform samplerCube prefilteredMap;
uniform sampler2D brdfLutMap;

vec3 computeSchlickApproximation(vec3 view_direction, vec3 half_way, vec3 R0)
{
    float cosTheta = max(dot(view_direction, half_way), 0.0f);
    return R0 + (1.0f - R0) * pow(1.0f - cosTheta, 5.0f);
}

vec3 computeSchlickApproximation(float cosTheta, vec3 R0, float roughness)
{
    return R0 + (max(vec3(1.0f - roughness), R0) - R0) * pow(1.0f - cosTheta, 5.0f);
}

float computeTrowbridgeReitzNDF(vec3 normal, vec3 half_way, float roughness)
{     
    float a = roughness * roughness;
    float NH = max(dot(normal, half_way), 0.0f);

    float num = a * a;
    float denom = (NH * NH) * (num - 1.0f) + 1.0f;
    denom = PI * denom * denom;

    return num / denom;
}

float computeSchlickBeckmannApproximation(vec3 normal, vec3 view_direction, float roughness)
{
    float k = roughness + 1.0f;
    k = k * k / 8.0f;

    float NV = max(dot(normal, view_direction), 0.0f);

    float num = NV;
    float denom = NV * (1.0f - k) + k;

    return num / denom;
}

float computeSmithGS(vec3 normal, vec3 view_direction, vec3 light_direction, float roughness)
{
    float G1 = computeSchlickBeckmannApproximation(normal, view_direction, roughness);
    float G2 = computeSchlickBeckmannApproximation(normal, light_direction, roughness);

    return G1 * G2;
}

void main()
{
    const bool useLighting = true;

    fragmentColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);       

    if (useLighting)
    {
        //vec3 albedo = vec3(1.0f, 0.71f, 0.29f);
        //vec3 albedo = vec3(0.5f, 0.0f, 0.0f);
        vec3 albedo = material.diffuseColor;
        //float metallic = 0.9f;
        //float roughness = 0.1f;
        float metallic = material.metallic;
        float roughness = material.roughness;
        float ambientOcclusion = 1.0f;

        vec3 normal = normalize(fs_in.fragmentNormal);
        vec3 viewDirection = normalize(fs_in.cameraPosition - fs_in.fragmentPosition);
        vec3 reflectionDirection = reflect(-viewDirection, normal);
        float w0 = max(dot(normal, viewDirection), 0.0f);

        vec3 R0 = vec3(0.04f);
        R0 = mix(R0, albedo, metallic);

        // Reflectance
        vec3 L0 = vec3(0.0f);
        for (int i = 0; i < lightsCount; i++)
        {
            // Radiance
            vec3 lightDirection = normalize(light[i].position - fs_in.fragmentPosition);
            vec3 halfWay = normalize(viewDirection + lightDirection);

            float dist = length(light[i].position - fs_in.fragmentPosition);
            float attenuation = 1.0f / (dist * dist);
            vec3 radiance = light[i].diffuseColor * light[i].power * attenuation;          

            // Cook-Torrance BRDF

            // Fresnel Equation
            vec3 R = computeSchlickApproximation(viewDirection, halfWay, R0);

            // Normal Distribution Function
            float NDF = computeTrowbridgeReitzNDF(normal, halfWay, roughness);

            // Geometric Shadowing
            float GS = computeSmithGS(normal, viewDirection, lightDirection, roughness);

            vec3 num = R * NDF * GS;            
            float wi = max(dot(normal, lightDirection), 0.0f);
            float denom = 4.0f * w0 * wi;

            vec3 specular = num / max(denom, 0.01f);

            vec3 ks = R;
            vec3 kd = vec3(1.0f) - ks;
            kd *= 1.0f - metallic;

            L0 += (kd * albedo / PI + specular) * radiance * wi;
        }

        float cosTheta = max(dot(normal, viewDirection), 0.0f);
        
        vec3 ks = computeSchlickApproximation(cosTheta, R0, roughness);
        vec3 kd = (1.0f - ks) * (1.0f - metallic);

        vec3 irradiance = texture(irradianceMap, normal).rgb;
        vec3 diffuseColor = irradiance * albedo;

        const float MAX_REFLECTION_LOD = 4.0f;
        vec3 prefilteredColor = textureLod(prefilteredMap, reflectionDirection, roughness * MAX_REFLECTION_LOD).rgb;
        vec2 BRDF = texture(brdfLutMap, vec2(cosTheta, roughness)).rg;
        
        vec3 specularColor = prefilteredColor * (ks * BRDF.x + BRDF.y);
        
        vec3 ambientColor = (kd * diffuseColor + specularColor) * ambientOcclusion;

        fragmentColor.rgb = ambientColor + L0;

        //vec3 ks = computeSchlickApproximation(viewDirection, normal, R0);
        //vec3 kd = 1.0f - ks;
        //kd *= 1.0f - metallic;

        //vec3 irradiance = texture(irradianceMap, normal).rgb;
        //vec3 diffuseColor = irradiance * albedo;
        //vec3 ambientColor = kd * diffuseColor * ambientOcclusion;

        //fragmentColor.rgb = ambientColor + L0;
    }
    else
    {
        // Without lighting

        if (useDiffuseMaps) 
        {
            for (int i = 0; i < diffuseMapsCount; i++)
            {
                fragmentColor.rgb += texture(diffuseMaps[i], fs_in.textureCoords).rgb;
            }
        }
        else
        {
            fragmentColor.rgb = material.diffuseColor;
        }       
    }
}