#version 330 core

out vec4 fragColor;

in vec2 fragTexCoords;

uniform sampler2D diffuseMap;

void main()
{
	fragColor = texture(diffuseMap, fragTexCoords);
}