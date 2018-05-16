#version 430

out vec4 fragmentColor;

in vec3 textureCoords;

const float PI = 22.0f / 7.0f;

uniform samplerCube envMap;

void main()
{
    vec3 normal = normalize(textureCoords);

    vec3 irradiance = vec3(0.0f);

    vec3 up = vec3(0.0f, 1.0f, 0.0f);
    vec3 right = cross(up, normal);
    up = cross(normal, right);

    float delta = 0.1f;
    float samples = 0.0f;

    for (float phi = 0.0f; phi < 2.0f * PI; phi += delta)
    {
        float sinPhi = sin(phi);
        float cosPhi = cos(phi);

        for (float theta = 0.0f; theta < 0.5f * PI; theta += delta)
        {
            float sinTheta = sin(theta);
            float cosTheta = cos(theta);

            vec3 tangentSpace = vec3(sinTheta * cosPhi, sinTheta * sinPhi, cosTheta);
            vec3 worldSpace = tangentSpace.x * right + tangentSpace.y * up + tangentSpace.z * normal;

            irradiance += texture(envMap, worldSpace).rgb * cosTheta * sinTheta;
            samples++;
        }
    }

    irradiance = PI * irradiance * (1.0f / samples);
    fragmentColor = vec4(irradiance, 1.0f);
}