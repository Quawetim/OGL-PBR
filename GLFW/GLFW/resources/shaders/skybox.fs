#version 430

in VS_OUT
{
    vec3 textureCoords;
} fs_in;

out vec3 fragmentColor;

uniform samplerCube envMap;

void main()
{
    fragmentColor = texture(envMap, fs_in.textureCoords).rgb;

    float exposure = 2.0f;
    //fragmentColor = vec3(1.0f) - exp(-fragmentColor * exposure);
}