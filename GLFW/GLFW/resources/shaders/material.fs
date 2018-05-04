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

uniform samplerCube envMap;

vec3 computePointLight(int id, vec3 normal, vec3 fragment_position, vec3 view_direction)
{
    vec3 lightDirection;
    float distanceFromLight;

    if (useNormalMaps)
    {
        lightDirection = normalize(fs_in.lightsPositionsTBN[id] - fragment_position);
        distanceFromLight = length(fs_in.lightsPositionsTBN[id] - fragment_position);
    }
    else
    {
        lightDirection = normalize(lights[id].position - fragment_position);
        distanceFromLight = length(lights[id].position - fragment_position);
    }  

    float diffuseFactor = max(dot(normal, lightDirection), 0.0f);
    float specularFactor;

    if (true)
    {
        // Blinn-Phong

        vec3 halfWayDirection = normalize(lightDirection + view_direction);
        specularFactor = pow(max(dot(normal, halfWayDirection), 0.0f), material.shininess * 1.56);
    }
    else
    {
        // Phong

        vec3 reflectionDirection = reflect(-lightDirection, normal);
        specularFactor = pow(max(dot(view_direction, reflectionDirection), 0.0f), material.shininess);
    }    

    vec3 ambientColor = vec3(0.0f);
    vec3 diffuseColor = vec3(0.0f);
    vec3 specularColor = vec3(0.0f);

    if (useDiffuseMaps)
    {
        for (int i = 0; i < diffuseMapsCount && i < MAX_MAPS; i++)
        {
            diffuseColor += texture(diffuseMaps[i], fs_in.textureCoords).rgb;
        }
        
        ambientColor = 0.05f * diffuseColor;;
        diffuseColor *= lights[id].diffuseColor * lights[id].power * diffuseFactor;
    }
    else
    {
        ambientColor = 0.05f * material.diffuseColor;
        diffuseColor = lights[id].diffuseColor * lights[id].power * material.diffuseColor * diffuseFactor;
    }

    if (useSpecularMaps)
    {
        for (int i = 0; i < specularMapsCount && i < MAX_MAPS; i++)
        {
            specularColor += texture(specularMaps[i], fs_in.textureCoords).rgb;
        }

        specularColor *= lights[id].specularColor * lights[id].power * specularFactor;
    }
    else
    {
        specularColor = lights[id].specularColor * lights[id].power * specularFactor;
    }

    float constant_factor = 1.0f; 
    float linear_factor = 2.0 / lights[id].radius;
    float quadratic_factor = 1.0 / (lights[id].radius * lights[id].radius);

    float attenuation = 1.0f / (constant_factor + (linear_factor * distanceFromLight) + (quadratic_factor * distanceFromLight * distanceFromLight));

    ambientColor *= attenuation;
    diffuseColor *= attenuation;
    specularColor *= attenuation;

    return ambientColor + diffuseColor + specularColor;   
}

vec3 computeRimLighting(int id, vec3 normal, vec3 view_direction)
{	
	float factor = 1.0f - max(dot(normal, view_direction), 0.0f);
	
	factor = smoothstep(0.0f, 1.0f, factor);
	
    float rimPower = 1.0f;

	factor = pow(factor, 1.0f / rimPower);
	
    vec3 color = factor * lights[id].diffuseColor * material.diffuseColor;
	
	return color;
}

vec4 computeReflection(vec3 normal, vec3 view_direction)
{
    vec3 R = reflect(-view_direction, normal);

    return texture(envMap, R);
}

vec4 computeRefraction(vec3 normal, vec3 view_direction)
{
    vec3 R = refract(-view_direction, normal, 1.0f / material.refractiveIndex);
    
    return texture(envMap, R);
}

void main()
{
    const bool useLighting = true;

    fragmentColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);       

    if (useLighting)
    {
        // With lighting
        
        vec3 normal, fragmentPosition, viewDirection;

        if (useNormalMaps)
        {           
            normal = texture(normalMaps[0], fs_in.textureCoords).rgb;
            normal = normalize(normal * 2.0f - 1.0f);

            fragmentPosition = fs_in.fragmentPositionTBN;

            viewDirection = normalize(fs_in.cameraPositionTBN - fs_in.fragmentPositionTBN);
        }
        else
        {            
            normal = normalize(fs_in.fragmentNormal);

            fragmentPosition = fs_in.fragmentPosition;

            viewDirection = normalize(fs_in.cameraPosition - fs_in.fragmentPosition);
        }

        for (int i = 0; i < lightsCount; i++)
        {
            fragmentColor.rgb += computePointLight(i, normal, fragmentPosition, viewDirection);
            
            //if (fs_in.lightsPositions[i].z < fs_in.fragmentPosition.z)
            //{
            //    fragmentColor.rgb += computeRimLighting(i, normal, viewDirection);
            //}
        }

        // Calculate refraction and reflection
        //if (material.refractiveIndex > 0 && material.reflectiveIndex > 0)
        //{
        //    vec4 reflection = computeReflection(normal, viewDirection);
        //    reflection.a = material.reflectiveIndex;
        //    //reflection.rgb = mix(reflection.rgb, fragmentColor.rgb, material.reflectiveIndex);

        //    vec4 refraction = computeRefraction(normal, viewDirection);
        //    refraction.a = 1.0f;
        //    //refraction.rgb = mix(refraction.rgb, fragmentColor.rgb, material.refractiveIndex);	
            
        //    fragmentColor = (reflection + refraction + fragmentColor) / 3.0f;
        
        //    //vec3 fresnel = vec3(dot(viewDirection, normal));
        //    //fragmentColor.rgb = mix(reflection, refraction, fresnel);
        //}
        //else
        //{
        //    if (material.reflectiveIndex > 0)
        //    {
        //        vec4 reflection = computeReflection(normal, viewDirection);
        //        reflection.a = material.reflectiveIndex;

        //        fragmentColor += reflection;
        //        //fragmentColor.rgb = mix(reflection.rgb, fragmentColor.rgb, material.reflectiveIndex);
        //    }

        //    if (material.refractiveIndex > 0)
        //    {
        //        vec4 refraction = computeRefraction(normal, viewDirection);
        //        refraction.a = 1.0f;

        //        fragmentColor += refraction;

        //        //fragmentColor.rgb = mix(refraction.rgb, fragmentColor.rgb, material.refractiveIndex);
        //    }       
        //}
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