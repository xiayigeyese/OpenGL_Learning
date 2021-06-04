#version 330 core

out vec4 fragColor;

in vec3 fragPos;
in vec3 fragNormal;
in vec2 fragTexCoords;
in vec4 lightSpacePos;

uniform sampler2D diffuseMap;
uniform sampler2D depthMap;
uniform vec3 lightPos;
uniform vec3 viewPos;

float calShadow(vec4 lightSpacePos, float bias)
{
    // clip space -> ndc
	vec3 clipPos = lightSpacePos.xyz / lightSpacePos.w;
	clipPos = clipPos * 0.5 + 0.5;
	float currentDepth = clipPos.z;
	float shadow = 0.0f;
	vec2 texelSize = 1.0 / textureSize(depthMap, 0);
	// pcf
	for(int i=-1;i<=1;i++)
	{
		for(int j = -1;j<=1;j++)
		{
			float closeDepth = texture(depthMap, clipPos.xy + vec2(i,j) * texelSize).r;
			// bias
			shadow += currentDepth - bias > closeDepth ? 1.0 : 0.0;
		}
	}
	shadow /= 9;
	return shadow;
}

void main()
{
    vec3 objectColor = texture(diffuseMap, fragTexCoords).rgb;
    objectColor = pow(objectColor, vec3(2.2f));
	vec3 lightColor = vec3(0.3);

	// ambient
	vec3 ambient = vec3(0.005f);

	// diffuse
	vec3 normal = normalize(fragNormal);
	vec3 lightDir = normalize(lightPos - fragPos);
	float diff = max(dot(lightDir, normal), 0);
	vec3 diffuse = diff * lightColor;

	// specular
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 halfWayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(halfWayDir, normal), 0), 64);
	vec3 specular = spec * lightColor;

	// shadow
	float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
	float shadow = calShadow(lightSpacePos, bias);

	vec3 result = (ambient + (1 - shadow) * (diffuse + specular)) * objectColor;
	// vec3 result = (ambient + diffuse + specular) * objectColor;
	
	result = pow(result, vec3(1.0f/2.2f));
	fragColor = vec4(result, 1.0f);
}