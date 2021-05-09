#version 330 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec3 gAlbedo;

in vec3 fragPos;
in vec3 fragNormal;
in vec2 fragTexCoords;

void main()
{
	gPosition = fragPos;
	gNormal = normalize(fragNormal);
	gAlbedo = vec3(0.95f);
}