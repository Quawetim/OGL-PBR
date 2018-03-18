#version 430 core

in vec2 TexCoords;

out vec4 Color;

uniform bool textureFlag;
uniform sampler2D diffuseMap1;

void main()
{	
	//Color = vec4(0.6f, 0.0f, 0.0f, 1.0f);

	vec2 size = vec2(1280.0f, 800.0f);
	vec2 position = (gl_FragCoord.xy / size.xy);

	vec4 top = vec4(0.878f, 0.105f, 0.666f, 1.0f);
	vec4 bottom = vec4(0.407f, 0.149f, 0.792f, 1.0f);

	//Color = vec4(mix(bottom, top, position.y));
   
    if (textureFlag) 
	{
		Color = texture(diffuseMap1, TexCoords);
	}
	else 
	{
		Color = vec4(mix(bottom, top, position.y));
	}
}