#version 330 core
out vec4 fragColor;

in vec3 fragPos;
in vec3 fragNormal;
in vec2 fragTexCoords;

struct Light 
{
	vec3 position;
	vec3 color;
};

uniform Light lights[4];
uniform sampler2D diffuseMap;

void main()
{
	vec3 objectColor = texture(diffuseMap, fragTexCoords).rgb;
	objectColor = pow(objectColor, vec3(2.2f));
	// ambient
	vec3 ambient = 0.0 * objectColor;
	// diffuse
	vec3 normal = normalize(fragNormal);
	vec3 diffuseLight = vec3(0);
	for(int i=0;i<4;i++)
	{
		vec3 lightDir = normalize(lights[i].position - fragPos);
		float diff = max(dot(lightDir, normal), 0);
		vec3 diffuse = diff * lights[i].color * objectColor;
		// diffuseLight += diffuse;
		float distance = length(lights[i].position - fragPos);
		diffuseLight += diffuse * 1.0 / (distance * distance);
	}
	fragColor = vec4(ambient + diffuseLight, 1.0);
}