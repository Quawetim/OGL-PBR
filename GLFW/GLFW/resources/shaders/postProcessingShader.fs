#version 430

in VS_OUT
{
    vec2 textureCoords;
} fs_in;

out vec4 fragmentColor;

uniform sampler2D frame;

void main()
{
    float gamma = 1.0f;
    int effect = 0;

    vec3 color;
    vec3 texelColor = texture(frame, fs_in.textureCoords).rgb;

    switch (effect)
    {
        default:
            {
                // no effects
                color = texelColor;                
                break;
            }
        case 1:
            {
                // grayscale
                float avg = (texelColor.r + texelColor.g + texelColor.b) / 3.0f;
                
                color = vec3(avg);                
                break;                
            }
        case 2:
            {
                // inverse
                color = 1.0f - texelColor;                
                break;
            }
        case 3:
            {
                // sepia
                float red = (texelColor.r * 0.393f) + (texelColor.g * 0.769f) + (texelColor.b * 0.189f);
                float green = (texelColor.r * 0.349f) + (texelColor.g * 0.686f) + (texelColor.b * 0.168f);
                float blue = (texelColor.r * 0.272f) + (texelColor.g * 0.534f) + (texelColor.b * 0.131f);
                
                color = vec3(red, green, blue);
                break;
            }
    }
    
    color = pow(color, vec3(1.0f / gamma));

    fragmentColor = vec4(color, 1.0f);
}