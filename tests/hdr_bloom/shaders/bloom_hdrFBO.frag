#version 330 core
layout (location = 0) out vec4 fragColor;
layout (location = 1) out vec4 prevBlurColor;

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

	vec3 ambient = 0.0 * objectColor;

	vec3 normal = normalize(fragNormal);
	vec3 diffuseLight = vec3(0);
	for(int i=0;i<4;i++)
	{
		vec3 lightDir = normalize(lights[i].position - fragPos);
		float diff = max(dot(lightDir, normal), 0);
		vec3 diffuse = diff * lights[i].color * objectColor;
		float distance = length(lights[i].position - fragPos);
		diffuseLight += diffuse * 1.0f / (distance * distance);
	}
	vec3 result = ambient + diffuseLight;
	// out fragColor 
	fragColor = vec4(result, 1.0f);
    // out lightColor for blur
    float brightness = dot(result, vec3(0.2126, 0.7152, 0.0722));
	if (brightness>1.0f) 
	{
		prevBlurColor = vec4(result, 1.0f);
	}
	else 
	{
	    prevBlurColor = vec4(0,0,0,1.0);
	}
}