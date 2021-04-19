#version 330 core

out vec4 fragColor;

in vec3 fragPos;
in vec3 fragNormal;
in vec2 fragTexCoords;

uniform sampler2D diffuseMap;
uniform vec3 lightPos;

void main()
{
    vec3 objectColor = texture(diffuseMap, fragTexCoords).rgb;
	objectColor = pow(objectColor,  vec3(2.2f));
	
	vec3 lightColor = vec3(0.5);

	vec3 normal = normalize(fragNormal);
	vec3 lightDir = normalize(lightPos - fragPos);
	float diff = max(dot(lightDir, normal), 0);
	vec3 diffuse = diff * objectColor * lightColor;

	diffuse = pow(diffuse, vec3(1.0f/2.2f));
	fragColor = vec4(diffuse, 1.0f);
}