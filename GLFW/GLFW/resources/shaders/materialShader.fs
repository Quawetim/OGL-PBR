#version 430

#define MAX_MAPS 5
#define MAX_LIGHTS 5

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

in VS_OUT
{
    vec3 fragmentPosition;
    vec3 fragmentNormal;
    vec2 textureCoords;
    
    vec3 cameraPosition;

    Light light;
} fs_in;

out vec3 fragmentColor;

uniform Material material;

uniform bool useDiffuseMaps;
uniform bool useSpecularMaps;
uniform bool useNormalMaps;

uniform sampler2D diffuseMaps[MAX_MAPS];
uniform int diffuseMapsCount;

uniform sampler2D specularMaps[MAX_MAPS];
uniform int specularMapsCount;

uniform sampler2D normalMaps[MAX_MAPS];
uniform int mormalMapsCount;

vec3 computePointLight(vec3 normal, vec3 fragment_position, vec3 view_direction)
{
    vec3 lightDirection = normalize(fs_in.light.position - fragment_position);

    float diff = max(dot(normal, lightDirection), 0.0f);
    float spec;

    if (true)
    {
        // Blinn-Phong

        vec3 halfWayDirection = normalize(lightDirection + view_direction);
        spec = pow(max(dot(normal, halfWayDirection), 0.0f), material.shininess * 4.0);
    }
    else
    {
        // Phong

        vec3 reflectionDirection = reflect(-lightDirection, normal);
        spec = pow(max(dot(view_direction, reflectionDirection), 0.0f), material.shininess);
    }    

    vec3 ambientColor, diffuseColor, specularColor;

    if (useDiffuseMaps)
    {
        ambientColor = texture(diffuseMaps[0], fs_in.textureCoords).rgb;
        diffuseColor = texture(diffuseMaps[0], fs_in.textureCoords).rgb;

        for (int i = 1; i < diffuseMapsCount && i < MAX_MAPS; i++)
        {
            ambientColor += texture(diffuseMaps[i], fs_in.textureCoords).rgb;
            diffuseColor += texture(diffuseMaps[i], fs_in.textureCoords).rgb;
        }
        
        ambientColor *= 0.05f;
        diffuseColor *= fs_in.light.diffuseColor * fs_in.light.power * diff;
    }
    else
    {
        ambientColor = material.ambientColor * material.diffuseColor;
        diffuseColor = fs_in.light.diffuseColor * fs_in.light.power * material.diffuseColor * diff;
    }

    if (useSpecularMaps)
    {
        specularColor = texture(specularMaps[0], fs_in.textureCoords).rgb;

        for (int i = 1; i < specularMapsCount && i < MAX_MAPS; i++)
        {
            specularColor += texture(specularMaps[i], fs_in.textureCoords).rgb;
        }

        specularColor *= fs_in.light.specularColor * fs_in.light.power * spec;
    }
    else
    {
        specularColor = fs_in.light.specularColor * fs_in.light.power * material.specularColor * spec;
    }

    float dist = length(fs_in.light.position - fragment_position);

    float constant_factor = 1.0f;
    float linear_factor = 2.0 / fs_in.light.radius;
    float quadratic_factor = 1.0 / (fs_in.light.radius * fs_in.light.radius);

    float attenuation = 1.0f / (constant_factor + (linear_factor * dist) + (quadratic_factor * dist * dist));

    ambientColor *= attenuation;
    diffuseColor *= attenuation;
    specularColor *= attenuation;

    return ambientColor + diffuseColor + specularColor;
}

void main()
{
    float gamma = 1.0f;

    if (true)
    {
        // With lighting

        vec3 normal;

        if (useNormalMaps)
        {
            normal = texture(normalMaps[0], fs_in.textureCoords).rgb;
            normal = normalize(normal * 2.0f - 1.0f);            
        }
        else
        {
            normal = normalize(fs_in.fragmentNormal);
        }       
        
        vec3 viewDirection = normalize(fs_in.cameraPosition - fs_in.fragmentPosition);

        fragmentColor = computePointLight(normal, fs_in.fragmentPosition, viewDirection);
    }
    else
    {
        // Without lighting

        if (useDiffuseMaps) 
        {            
            fragmentColor = texture(diffuseMaps[0], fs_in.textureCoords).rgb;

            for (int i = 1; i < diffuseMapsCount && i < MAX_MAPS; i++)
            {
                fragmentColor += texture(diffuseMaps[i], fs_in.textureCoords).rgb;
            }
        }
        else fragmentColor = material.diffuseColor;
    }

    fragmentColor = pow(fragmentColor, vec3(1.0f / gamma));
}