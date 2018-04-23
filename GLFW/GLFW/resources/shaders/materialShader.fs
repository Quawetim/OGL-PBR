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

    vec3 lightsPositions[MAX_LIGHTS];
} fs_in;

out vec3 fragmentColor;

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
uniform Light lights[MAX_LIGHTS];

vec3 computePointLight(int id, vec3 normal, vec3 fragment_position, vec3 view_direction)
{
    vec3 lightDirection;

    if (useNormalMaps)
    {
        lightDirection = normalize(fs_in.lightsPositions[id] - fragment_position);
    }
    else
    {
        lightDirection = normalize(lights[id].position - fragment_position);
    }

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
        diffuseColor *= lights[id].diffuseColor * lights[id].power * diff;
    }
    else
    {
        ambientColor = material.ambientColor * material.diffuseColor;
        diffuseColor = lights[id].diffuseColor * lights[id].power * material.diffuseColor * diff;
    }

    if (useSpecularMaps)
    {
        specularColor = texture(specularMaps[0], fs_in.textureCoords).rgb;

        for (int i = 1; i < specularMapsCount && i < MAX_MAPS; i++)
        {
            specularColor += texture(specularMaps[i], fs_in.textureCoords).rgb;
        }

        specularColor *= lights[id].specularColor * lights[id].power * spec;
    }
    else
    {
        specularColor = lights[id].specularColor * lights[id].power * material.specularColor * spec;
    }

    float dist;
    
    if (useNormalMaps)
    {
        dist = length(fs_in.lightsPositions[id] - fragment_position);
    }
    else
    {
        dist = length(lights[id].position - fragment_position);
    }

    float attenuation;

    if (true)
    {
        float constant_factor = 1.64f; 
        float linear_factor = 2.0 / lights[id].radius;
        float quadratic_factor = 1.0 / (lights[id].radius * lights[id].radius);

        attenuation = 1.0f / (constant_factor + (linear_factor * dist) + (quadratic_factor * dist * dist));
    }
    else
    {
        float d = max(dist - lights[id].radius, 0);
        lightDirection /= dist;
     
        // calculate basic attenuation
        float denom = d / lights[id].radius + 1;
        attenuation = 1 / (denom*denom);
     
        // scale and bias attenuation such that:
        //   attenuation == 0 at extent of max influence
        //   attenuation == 1 when d == 0
        attenuation = (attenuation - 0.001f) / (1 - 0.001f);
        attenuation = max(attenuation, 0);
    }

    ambientColor *= attenuation;
    diffuseColor *= attenuation;
    specularColor *= attenuation;

    return ambientColor + diffuseColor + specularColor;   
}

void main()
{
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

        vec3 color = vec3(0.0f, 0.0f, 0.0f);

        for (int i = 0; i < lightsCount; i++)
        {
            color += computePointLight(i, normal, fs_in.fragmentPosition, viewDirection);
        }

        fragmentColor = color;
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
}