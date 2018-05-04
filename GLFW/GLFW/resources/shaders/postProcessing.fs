#version 430

in VS_OUT
{
    vec2 textureCoords;
} fs_in;

out vec4 fragmentColor;

uniform float gamma;

uniform sampler2D frame;

void main()
{
    int effect = 0;
    bool vignette = true;

    fragmentColor = texture(frame, fs_in.textureCoords);

    switch (effect)
    {
        default:
            {
                // no effects
                break;
            }
        case 1:
            {
                // grayscale
                float avg = (fragmentColor.r + fragmentColor.g + fragmentColor.b) / 3.0f;
                
                fragmentColor.rgb = vec3(avg);                
                break;                
            }
        case 2:
            {
                // inverse
                fragmentColor.rgb = 1.0f - fragmentColor.rgb;                
                break;
            }
        case 3:
            {
                // sepia
                float red = (fragmentColor.r * 0.393f) + (fragmentColor.g * 0.769f) + (fragmentColor.b * 0.189f);
                float green = (fragmentColor.r * 0.349f) + (fragmentColor.g * 0.686f) + (fragmentColor.b * 0.168f);
                float blue = (fragmentColor.r * 0.272f) + (fragmentColor.g * 0.534f) + (fragmentColor.b * 0.131f);
                
                fragmentColor.rgb = vec3(red, green, blue);
                break;
            }           
    }

    if (vignette)
    {	
        vec2 resolution = vec2(1280.0f, 720.0f);
	    vec2 position = (gl_FragCoord.xy / resolution.xy) - vec2(0.5f);

	    float len = length(position);
	            
        float radius = 0.8f;
        float softness = 0.5f;

        float vignette = smoothstep(radius, radius - softness, len);

        fragmentColor.rgb *= vignette;
    }

    //fragmentColor.rgb = fragmentColor.rgb / (fragmentColor.rgb + vec3(1.0));

    float exposure = 2.0f;
    fragmentColor.rgb = vec3(1.0f) - exp(-fragmentColor.rgb * exposure);
    
    fragmentColor.rgb = pow(fragmentColor.rgb, vec3(1.0f / gamma));
}