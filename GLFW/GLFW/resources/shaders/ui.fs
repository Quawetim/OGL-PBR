#version 430 core

in VS_OUT
{
    vec2 textureCoords;
} fs_in;

out vec4 fragmentColor;

uniform vec3 color;
uniform bool useTexture;
uniform sampler2D Texture;

uniform bool text;

void main()
{	
    if (text)
    {
        vec4 sampled = vec4(1.0f, 1.0f, 1.0f, texture(Texture, fs_in.textureCoords).r);
	    fragmentColor = vec4(color, 1.0f) * sampled;
    }
    else
    {
        if (useTexture)
        {
            fragmentColor = texture(Texture, fs_in.textureCoords);
            fragmentColor.rgb *= color;
        }
        else
        {
            fragmentColor = vec4(color, 1.0f);
        }
    }
}