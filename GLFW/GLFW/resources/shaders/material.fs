#version 430

#define MAX_MAPS 5
#define MAX_LIGHTS 5

struct Material
{
    vec3 albedo;
    float metallic;
    float smoothness;
    float surfaceHeight;
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

uniform bool useAmbientOcclusionMaps;
uniform int ambientOcclusionMapsCount;
uniform sampler2D ambientOcclusionMaps[MAX_MAPS];

uniform bool useNormalMaps;
uniform int normalMapsCount;
uniform sampler2D normalMaps[MAX_MAPS];

uniform bool useHeightMaps;
uniform int heightMapsCount;
uniform sampler2D heightMaps[MAX_MAPS];

uniform int lightsCount;
uniform Light light[MAX_LIGHTS];

uniform samplerCube environmentMap;
uniform samplerCube irradianceMap;
uniform samplerCube prefilteredMap;
uniform sampler2D brdfLutMap;

mat3 computeTBN()
{
    vec3 Q1 = dFdx(fs_in.fragmentPosition);
    vec3 Q2 = dFdy(fs_in.fragmentPosition);
    vec2 st1 = dFdx(fs_in.textureCoords);
    vec2 st2 = dFdy(fs_in.textureCoords);

    vec3 N = normalize(fs_in.fragmentNormal);
    vec3 T = normalize(Q1 * st2.t - Q2 * st1.t);
    vec3 B = -normalize(cross(N, T));

    return mat3(T, B, N);
}

vec3 getNormal(int map, mat3 TBN, vec2 texture_coords)
{
    vec3 normal = texture(normalMaps[map], texture_coords).rgb * 2.0f - 1.0f;   

    return normalize(TBN * normal);
}

vec3 computePointLight(int id, vec3 normal, vec3 fragment_position, vec3 view_direction)
{
    vec3 lightDirection;
    float distanceFromLight;

    lightDirection = normalize(light[id].position - fragment_position);
    distanceFromLight = length(light[id].position - fragment_position);

    float diffuseFactor = max(dot(normal, lightDirection), 0.0f);
    float specularFactor;

    if (true)
    {
        // Blinn-Phong

        vec3 halfWayDirection = normalize(lightDirection + view_direction);
        specularFactor = pow(max(dot(normal, halfWayDirection), 0.0f), 512.0f);
    }
    else
    {
        // Phong

        vec3 reflectionDirection = reflect(-lightDirection, normal);
        specularFactor = pow(max(dot(view_direction, reflectionDirection), 0.0f), 256.0f);
    }    

    vec3 ambientColor = vec3(0.0f);
    vec3 diffuseColor = vec3(0.0f);
    vec3 specularColor = vec3(0.0f);

    if (useAlbedoMaps)
    {
        for (int i = 0; i < albedoMapsCount && i < MAX_MAPS; i++)
        {
            diffuseColor += texture(albedoMaps[i], fs_in.textureCoords).rgb;
        }
        
        ambientColor = 0.05f * diffuseColor;
        diffuseColor *= light[id].diffuseColor * light[id].power * diffuseFactor;
    }
    else
    {
        ambientColor = 0.05f * material.albedo;
        diffuseColor = light[id].diffuseColor * light[id].power * material.albedo * diffuseFactor;
    }

    specularColor = light[id].specularColor * light[id].power * specularFactor;

    float constant_factor = 1.0f; 
    float linear_factor = 2.0 / light[id].radius;
    float quadratic_factor = 1.0 / (light[id].radius * light[id].radius);

    float attenuation = 1.0f / (constant_factor + (linear_factor * distanceFromLight) + (quadratic_factor * distanceFromLight * distanceFromLight));

    ambientColor *= attenuation;
    diffuseColor *= attenuation;
    specularColor *= attenuation;

    //return ambientColor;
    //return diffuseColor;
    //return ambientColor + diffuseColor;
    //return specularColor;
    return ambientColor + diffuseColor + specularColor;
    
}

vec3 computeRimLighting(int id, vec3 normal, vec3 view_direction)
{	
	float factor = 1.0f - max(dot(normal, view_direction), 0.0f);
	
	factor = smoothstep(0.0f, 1.0f, factor);
	
    float rimPower = 1.0f;

	factor = pow(factor, 1.0f / rimPower);
	
    vec3 color = factor * light[id].diffuseColor * material.albedo;
	
	return color;
}

vec4 computeReflection(vec3 normal, vec3 view_direction)
{
    vec3 R = reflect(-view_direction, normal);

    return texture(environmentMap, R);
}

void main()
{
    fragmentColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);       

    if (true)
    {
        // With lighting
        
        vec3 normal;

        if (useNormalMaps)
        {           
            mat3 TBN = computeTBN();
            normal = getNormal(0, TBN, fs_in.textureCoords);
        }
        else
        {            
            normal = normalize(fs_in.fragmentNormal);           
        }

        vec3 viewDirection = normalize(fs_in.cameraPosition - fs_in.fragmentPosition);

        for (int i = 0; i < lightsCount; i++)
        {
            fragmentColor.rgb += computePointLight(i, normal, fs_in.fragmentPosition, viewDirection);
        }

        vec4 reflection = 0.5f * computeReflection(normal, viewDirection);

        if (useMetallicMaps)
        {
            float metallic = 0.5f * texture(metallicMaps[0], fs_in.textureCoords).r;
            fragmentColor.rgb = mix(fragmentColor.rgb, reflection.rgb, metallic);
        }
        else
        {
            fragmentColor.rgb = mix(fragmentColor.rgb, reflection.rgb, material.metallic);
        }
    }
    else
    {
        // Without lighting

        if (useAlbedoMaps) 
        {
            for (int i = 0; i < albedoMapsCount; i++)
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