#version 430

out vec4 fragmentColor;

in vec3 textureCoords;

const float PI = 22.0f / 7.0f;
//const int MAX_SAMPLES = 1024;

uniform samplerCube envMap;
uniform float roughness;

float VanDerCorputSequence(int n)
{
    float res = 0.0f;
    float inv = 1.0f / 2.0f;

    for (int i = 0; i < 32; i++)
    {
        if (n > 0)
        {
            res += mod(float(n), 2.0f) * inv;
            inv = inv / 2.0f;
            n = int(float(n) / 2.0f);
        }
    }

    return res;
}

vec2 HammersleySequence(int i, int n)
{
    float k = float(i) / float(n);

    return vec2(k, VanDerCorputSequence(i));
}

vec3 ImportanceSamplingGGX(vec2 Xi, vec3 normal, float roughness)
{
    float a = roughness * roughness;

    float phi = 2.0f * PI * Xi.x;
    float cosTheta = sqrt((1.0f - Xi.y) / (1.0f + (a * a - 1.0f) * Xi.y));
    float sinTheta = sqrt(1.0f - cosTheta * cosTheta);

    vec3 H;
    H.x = cos(phi) * sinTheta;
    H.y = sin(phi) * sinTheta;
    H.z = cosTheta;

    vec3 up;

    if (abs(normal.z) < 0.999f)
    {
        up = vec3(0.0f, 0.0f, 1.0f);
    }
    else
    {
        up = vec3(1.0f, 0.0f, 0.0f);
    }

    vec3 tangent = normalize(cross(up, normal));
    vec3 bitangent = cross(normal, tangent);

    vec3 sampledVector = normalize(tangent * H.x + bitangent * H.y + normal * H.z);
    
    return sampledVector;
}

void main()
{
    vec3 N = normalize(textureCoords);
    vec3 R = N;
    vec3 V = R;

    float weight = 0.0f;
    vec3 color = vec3(0.0f);

    for (int i = 0; i < 512; i++)
    {
        vec2 Xi = HammersleySequence(i, 512);
        vec3 H = ImportanceSamplingGGX(Xi, N, roughness);
        vec3 L = normalize(2.0f * dot(V, H) * H - V);

        float NL = max(dot(N, L), 0.0f);

        if (NL > 0.0f)
        {
            color += texture(envMap, L).rgb * NL;
            weight += NL;
        }
    }

    fragmentColor = vec4(color / weight, 1.0f);
}