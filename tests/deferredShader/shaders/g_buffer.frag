#version 330 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

in vec3 fragPos;
in vec3 fragNormal;
in vec2 fragTexCoords;

uniform sampler2D diffuseMap;
uniform sampler2D specularMap;

void main()
{
	gAlbedoSpec.rgb = texture(diffuseMap, fragTexCoords).rgb;
	gAlbedoSpec.a = texture(specularMap, fragTexCoords).r;
	gPosition = fragPos;
	gNormal = normalize(fragNormal);
}