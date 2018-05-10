#version 430

#define MAX_MAPS 5
#define MAX_LIGHTS 5

struct Material
{
    vec3 albedo;
    float metallic;
    float smoothness;    
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
    vec3 fragmentNormal;
    vec2 textureCoords;
    vec3 cameraPosition; 
} fs_in;

out vec4 fragmentColor;

const float PI = 22.0f / 7.0f;
const float MAX_REFLECTION_LOD = 4.0f;

uniform Material material;

uniform bool useAlbedoMaps;
uniform int albedoMapsCount;
uniform sampler2D albedoMaps[MAX_MAPS];

uniform bool useSmoothnessMaps;
uniform int smoothnessMapsCount;
uniform sampler2D smoothnessMaps[MAX_MAPS];

uniform bool useMetallicMaps;
uniform int metallicMapsCount;
uniform sampler2D metallicMaps[MAX_MAPS];

uniform bool useNormalMaps;
uniform int normalMapsCount;
uniform sampler2D normalMaps[MAX_MAPS];

uniform int lightsCount;
uniform Light light[MAX_LIGHTS];

uniform samplerCube environmentMap;
uniform samplerCube irradianceMap;
uniform samplerCube prefilteredMap;
uniform sampler2D brdfLutMap;

vec3 getNormal(int map)
{
    vec3 normal = texture(normalMaps[map], fs_in.textureCoords).rgb * 2.0f - 1.0f;

    vec3 Q1 = dFdx(fs_in.fragmentPosition);
    vec3 Q2 = dFdy(fs_in.fragmentPosition);
    vec2 st1 = dFdx(fs_in.textureCoords);
    vec2 st2 = dFdy(fs_in.textureCoords);

    vec3 N = normalize(fs_in.fragmentNormal);
    vec3 T = normalize(Q1 * st2.t - Q2 * st1.t);
    vec3 B = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * normal);
}

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

float computeAmbientOcclusion(vec3 fragment_position, vec3 normal, vec3 view_direction)
{   
    float total = 0.0f;
    float ao = 0.0f;
 
    for (int i = 0; i < 16; i ++)
    {
        float cosTheta = dot(normalize(view_direction), normal);
 
        if (cosTheta > 0.01f && dot(view_direction, view_direction) < 100.0f)
        {
            ao += sin(2.0f * acos(cosTheta));
            total += 1.0f;
        }
    }
 
    ao = 1.0f - ao / (2.0f * total + 0.01f);

    return ao;
}

void main()
{
    const bool useLighting = true;

    fragmentColor = vec4(0.0f, 0.0f, 0.0f, 1.0f); 
     

    if (useLighting)
    {
        vec3 normal = vec3(0.0f);
        vec3 albedo = vec3(0.0f);    
        float smoothness = 0.0f;
        float metallic = 0.0f;

        ////////////////////////Albedo////////////////////////

        if (useAlbedoMaps)
        {
            for (int i = 0; i < albedoMapsCount && i <= MAX_MAPS; i++) albedo += texture(albedoMaps[i], fs_in.textureCoords).rgb;
        }
        else albedo = material.albedo;

        ////////////////////////Smoothness////////////////////////
        
        if (useSmoothnessMaps)
        {
            for (int i = 0; i < smoothnessMapsCount && i <= MAX_MAPS; i++) smoothness += texture(smoothnessMaps[i], fs_in.textureCoords).r;
        }
        else smoothness = material.smoothness;
        
        float roughness = 1.0f - smoothness;

        ////////////////////////Metallic////////////////////////

        if (useMetallicMaps)
        {
            for (int i = 0; i < metallicMapsCount && i <= MAX_MAPS; i++) metallic += texture(metallicMaps[i], fs_in.textureCoords).r;
        }
        else metallic = material.metallic;

        ////////////////////////Normal////////////////////////

        if (useNormalMaps)
        {
            for (int i = 0; i < normalMapsCount && i <= MAX_MAPS; i++) normal += getNormal(i);
        }
        else normal = normalize(fs_in.fragmentNormal);
 
        vec3 viewDirection = normalize(fs_in.cameraPosition - fs_in.fragmentPosition);
        vec3 reflectionDirection = reflect(-viewDirection, normal);
        float w0 = max(dot(normal, viewDirection), 0.0f);

        float ambientOcclusion = computeAmbientOcclusion(fs_in.fragmentPosition, normal, viewDirection);

        vec3 R0 = vec3(0.04f);
        R0 = mix(R0, albedo, metallic);

        // Reflectance
        vec3 L0 = vec3(0.0f);
        for (int i = 0; i < lightsCount && i <= MAX_LIGHTS; i++)
        {
            // Radiance
            vec3 lightDirection = normalize(light[i].position - fs_in.fragmentPosition);
            vec3 halfWay = normalize(viewDirection + lightDirection);

            float distanceFromLight = length(light[i].position - fs_in.fragmentPosition);
            
            // Atenuation
            float constant_factor = 1.0f; 
            float linear_factor = 2.0 / light[i].radius;
            float quadratic_factor = 1.0 / (light[i].radius * light[i].radius);

            float attenuation = 1.0f / (constant_factor + (linear_factor * distanceFromLight) + (quadratic_factor * distanceFromLight * distanceFromLight));            

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
            vec3 kd = (1.0f - ks) * (1.0f - metallic);

            L0 += (kd * albedo / PI + specular) * radiance * wi;
        }

        float cosTheta = max(dot(normal, viewDirection), 0.0f);
        
        vec3 ks = computeSchlickApproximation(cosTheta, R0, roughness);
        vec3 kd = (1.0f - ks) * (1.0f - metallic);

        vec3 irradiance = texture(irradianceMap, normal).rgb;
        vec3 diffuseColor = irradiance * albedo;
        
        vec3 prefilteredColor = textureLod(prefilteredMap, reflectionDirection, roughness * MAX_REFLECTION_LOD).rgb;
        vec2 BRDF = texture(brdfLutMap, vec2(cosTheta, roughness)).rg;
        
        vec3 reflectionColor = prefilteredColor * (ks * BRDF.x + BRDF.y);
        
        vec3 ambientColor = (kd * diffuseColor + reflectionColor) * ambientOcclusion;     

        fragmentColor.rgb = ambientColor + L0;
        //fragmentColor.rgb = normal;
        //fragmentColor.rgb = albedo;
        //fragmentColor.rgb = vec3(smoothness);
        //fragmentColor.rgb = vec3(roughness);
        //fragmentColor.rgb = vec3(metallic);
        //fragmentColor.rgb = vec3(ambientOcclusion);
    }
    else
    {
        // Without lighting

        if (useAlbedoMaps) 
        {
            for (int i = 0; i < albedoMapsCount && i <= MAX_MAPS; i++)
            {
                fragmentColor.rgb += texture(albedoMaps[i], fs_in.textureCoords).rgb;
            }
        }
        else
        {
            fragmentColor.rgb = material.albedo;
        }       
    }
}