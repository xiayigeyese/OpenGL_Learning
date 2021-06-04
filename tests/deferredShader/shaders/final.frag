#version 330 core
out vec4 fragColor;

in vec2 fragTexCoords;

uniform sampler2D deferredLightFBO;

void main()
{
	fragColor = texture(deferredLightFBO, fragTexCoords);
}