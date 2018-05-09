#version 430 core

in VS_OUT
{
    vec2 textureCoords;
} fs_in;

out vec2 fragmentColor;

const float PI = 22.0f / 7.0f;
const int MAX_SAMPLES = 1024;

float computeSchlickBeckmannApproximation(vec3 normal, vec3 view_direction, float roughness)
{
    float k = roughness * roughness / 2.0f;

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

    vec3 up = abs(normal.z) < 0.999f ? vec3(0.0f, 0.0f, 1.0f) : vec3(1.0f, 0.0f, 0.0f);
    vec3 tangent = normalize(cross(up, normal));
    vec3 bitangent = cross(normal, tangent);

    vec3 sampledVector = tangent * H.x + bitangent * H.y + normal * H.z;
    
    return normalize(sampledVector);
}

vec2 computeBRDF(float NV, float roughness)
{
    vec3 V;
    V.x = sqrt(1.0f - NV *NV);
    V.y = 0.0f;
    V.z = NV;

    float red = 0.0f;
    float green = 0.0f;

    vec3 N = vec3(0.0f, 0.0f, 1.0f);

    for (int i = 0; i < MAX_SAMPLES; i++)
    {
        vec2 Xi = HammersleySequence(i, MAX_SAMPLES);
        vec3 H = ImportanceSamplingGGX(Xi, N, roughness);
        vec3 L = normalize(2.0f * dot(V, H) * H - V);

        float NL = max(L.z, 0.0f);
        float NH = max(H.z, 0.0f);
        float VH = max(dot(V, H), 0.0f);

        if (NL > 0.0f)
        {
            float G = computeSmithGS(N, V, L, roughness);
            float GV = (G * VH) / (NH * NV);
            float F = pow(1.0f - VH, 5.0f);

            red += (1.0f - F) * GV;
            green += F * GV;
        }
    }

    red /= float(MAX_SAMPLES);
    green /= float(MAX_SAMPLES);

    return vec2(red, green);
}

void main()
{	
    fragmentColor = computeBRDF(fs_in.textureCoords.x, fs_in.textureCoords.y);
}