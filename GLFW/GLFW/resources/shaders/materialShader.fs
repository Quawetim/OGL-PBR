#version 430

struct QMaterial
{
    vec3 ambientColor;
    vec3 diffuseColor;
    vec3 specularColor;
    float shinePower;
};

in vec3 fragmentNormal;
in vec3 fragmentPosition;
in vec2 textureCoords;

out vec3 fragmentColor;

uniform QMaterial material;

uniform bool diffuseMap1_flag;
uniform bool specularMap1_flag;
uniform bool normalMap1_flag;

uniform sampler2D diffuseMap1;
uniform sampler2D specularMap1;
uniform sampler2D normalMap1;

vec3 computePointLight(int id, vec3 normal, vec3 fragpos, vec3 viewdir)
{
    vec3 light_position = vec3(0.0f, 0.0f, 15.0f);
    vec3 light_color = vec3(0.8f, 0.8f, 0.92f);
    float light_power = 10.0f;

    vec3 light_direction = normalize(light_position - fragpos);

    float diff = max(dot(normal, light_direction), 0.0f);
    float spec;

    bool blinn_phong = true;
    if (blinn_phong)
    {
        vec3 half_way_direction = normalize(light_direction + viewdir);
        spec = pow(max(dot(normal, half_way_direction), 0.0f), material.shinePower * 8.0f);
    }
    else
    {
        vec3 reflection_direction = reflect(-light_direction, normal);
        spec = pow(max(dot(viewdir, reflection_direction), 0.0f), material.shinePower);
    }    

    vec3 ambient_color, diffuse_color, specular_color;

    if (diffuseMap1_flag)
    {
        ambient_color = 0.05f * texture(diffuseMap1, textureCoords).rgb;
        diffuse_color = light_color * light_power * texture(diffuseMap1, textureCoords).rgb * diff;
    }
    else
    {
        ambient_color = material.ambientColor * material.diffuseColor;
        diffuse_color = light_color * light_power * material.diffuseColor * diff;
    }

    if (specularMap1_flag)
    {
        specular_color = light_color * light_power * texture(specularMap1, textureCoords).rgb * spec;
    }
    else
    {
        specular_color = light_color * light_power * material.specularColor * spec;
    }

    float dist = length(light_position - fragpos);
    float attenuation = 1.0f / (1.0f + 0.09f * dist + 0.032f * dist * dist);

    ambient_color *= attenuation;
    diffuse_color *= attenuation;
    specular_color *= attenuation;

    return ambient_color + diffuse_color + specular_color;
}

void main()
{
    float gamma = 1.0f;

    if (true)
    {
        vec3 cameraPosition = vec3(0.0f, 0.0f, 20.0f);
        vec3 normal = normalize(fragmentNormal);
        vec3 viewdir = normalize(cameraPosition - fragmentPosition);

        fragmentColor = computePointLight(0, normal, fragmentPosition, viewdir);
    }
    else
    {
        if (diffuseMap1_flag) fragmentColor = texture(diffuseMap1, textureCoords).rgb;
        else fragmentColor = material.diffuseColor;
    }

    fragmentColor = pow(fragmentColor, vec3(1.0f / gamma));
}