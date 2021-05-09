#version 330 core

out vec3 fragColor;

in vec2 fragTexCoords;

uniform sampler2D inputImage;

void main()
{
	vec2 texelSize = 1.0f / vec2(textureSize(inputImage, 0));
	vec3 result = vec3(0);
	for(int i=-2; i<=2; i++)
	{
		for(int j = -2; j<=2; j++)
		{
			result += texture(inputImage, fragTexCoords + vec2(i, j) * texelSize).rgb;
		}
	}
	fragColor = result /(5.0f * 5.0f);
}