#version 430

struct QMaterial
{
    vec3 ambientColor;
    vec3 diffuseColor;
    vec3 specularColor;
    float shininess;
};

in vec3 fragmentNormal;
in vec3 fragmentPosition;
in vec2 textureCoords;

out vec3 fragmentColor;

uniform QMaterial material;

uniform bool diffuseMap1_flag;
uniform bool specularMap1_flag;
uniform bool normalMap1_flag;

uniform vec3 cameraPosition;

uniform sampler2D diffuseMap1;
uniform sampler2D specularMap1;
uniform sampler2D normalMap1;

vec3 computePointLight(int id, vec3 normal, vec3 fragment_position, vec3 view_direction)
{
    vec3 lightPosition = vec3(0.0f, 0.0f, 15.0f);
    vec3 lightDiffuseColor = vec3(0.8f, 0.8f, 0.92f);
    vec3 lightSpecularColor = vec3(0.7f, 0.7f, 0.7f);
    float lightPower = 460.0f;
    float lightRadius = 1.0f;

    vec3 lightDirection = normalize(lightPosition - fragment_position);

    float diff = max(dot(normal, lightDirection), 0.0f);
    float spec;

    bool isBlinnPhong = true;

    if (isBlinnPhong)
    {
        vec3 halfWayDirection = normalize(lightDirection + view_direction);
        spec = pow(max(dot(normal, halfWayDirection), 0.0f), material.shininess * 4.0);
    }
    else
    {
        vec3 reflectionDirection = reflect(-lightDirection, normal);
        spec = pow(max(dot(view_direction, reflectionDirection), 0.0f), material.shininess);
    }    

    vec3 ambientColor, diffuseColor, specularColor;

    if (diffuseMap1_flag)
    {
        ambientColor = 0.05f * texture(diffuseMap1, textureCoords).rgb;
        diffuseColor = lightDiffuseColor * lightPower * texture(diffuseMap1, textureCoords).rgb * diff;
    }
    else
    {
        ambientColor = material.ambientColor * material.diffuseColor;
        diffuseColor = lightDiffuseColor * lightPower * material.diffuseColor * diff;
    }

    if (specularMap1_flag)
    {
        specularColor = lightSpecularColor * lightPower * texture(specularMap1, textureCoords).rgb * spec;
    }
    else
    {
        specularColor = lightSpecularColor * lightPower * material.specularColor * spec;
    }

    float dist = length(lightPosition - fragment_position);

    float constant_factor = 1.0f;
    float linear_factor = 2.0 / lightRadius;
    float quadratic_factor = 1.0 / (lightRadius * lightRadius);

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
        vec3 normal = normalize(fragmentNormal);
        vec3 viewDirection = normalize(cameraPosition - fragmentPosition);

        fragmentColor = computePointLight(0, normal, fragmentPosition, viewDirection);
    }
    else
    {
        if (diffuseMap1_flag) fragmentColor = texture(diffuseMap1, textureCoords).rgb;
        else fragmentColor = material.diffuseColor;
    }

    fragmentColor = pow(fragmentColor, vec3(1.0f / gamma));
}