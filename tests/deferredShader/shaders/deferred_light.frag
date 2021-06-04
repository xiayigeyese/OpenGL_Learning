#version 330 core
out vec4 fragColor;

in vec2 fragTexCoords;

struct Light
{
	vec3 position;
	vec3 color;
};

uniform Light lights[4];
uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

uniform vec3 viewPos;
uniform vec3 lightAttenuation;

void main()
{
	vec3 fragPosition = texture(gPosition, fragTexCoords).rgb;
	vec3 fragNormal = texture(gNormal, fragTexCoords).rgb;
	vec4 albedoSpec = texture(gAlbedoSpec, fragTexCoords);
	vec3 diffuseColor = albedoSpec.rgb;
	diffuseColor = pow(diffuseColor, vec3(2.2f));
	float specularCoefficient = albedoSpec.a;

	// ambient
	vec3 ambient = vec3(0.01) * diffuseColor;

	// diffuse + specular
	vec3 diffuseLight = vec3(0);
	vec3 specularLight = vec3(0);
	vec3 viewDir = normalize(viewPos - fragPosition);
	for(int i=0;i<15;i++)
	{
		// light attenuation
		float dist = distance(fragPosition, lights[i].position);
		float attenuation = 1.0 / (lightAttenuation[0] + lightAttenuation[1] * dist + lightAttenuation[2] * dist * dist);
		// float attenuation = 1.0 / (lightAttenuation[2] * dist * dist);
		// diffuse
		vec3 lightDir = normalize(lights[i].position - fragPosition);
		float diff = max(dot(lightDir, fragNormal), 0);
		vec3 diffuse = diff * diffuseColor * lights[i].color;
		diffuseLight += diffuse * attenuation;
        // specular
		vec3 halfWay = normalize(viewDir + lightDir);
		float spec = pow(max(dot(halfWay, fragNormal), 0), 32);
		vec3 specular = spec * specularCoefficient * lights[i].color;
		specularLight = specular * attenuation;
	}

	vec3 result = ambient + diffuseLight + specularLight;
	result = pow(result, vec3(1.0 / 2.2));
	fragColor = vec4(result, 1.0f);
}