#version 330 core
out vec4 fragColor;

in vec3 fragPos;
in vec3 fragNormal;
in vec2 fragTexCoords;

uniform sampler2D diffuseMap;
uniform samplerCube shadowMap;
uniform float farPlane;
uniform vec3 lightPos;
uniform vec3 viewPos;

float calculateShadow(float bias)
{
	// direction : releated to lightPos;
	vec3 fragDrection = fragPos - lightPos;
	float closeDepth = texture(shadowMap, fragDrection).r * farPlane;
	// depth
	float currentDepth = length(fragDrection);
	float shadow = (currentDepth - bias) > closeDepth ? 1.0 : 0.0;
	return shadow;
}

float calPCFShadow(float bias, float viewDistance)
{
	
	vec3 sampleOffsetDirections[20] = vec3[]
	(
	   vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1), 
	   vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
	   vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
	   vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
	   vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
	);
	int samples = 20;
	vec3 fragDrection = fragPos - lightPos;
	float diskRadius = (1.0 + (viewDistance / farPlane)) / farPlane;
	float shadow =0.0f;
	float currentDepth = length(fragDrection);
	for(int i=0;i<samples;i++)
	{
		float closeDepth = texture(shadowMap, fragDrection + sampleOffsetDirections[i] * diskRadius).r;
		closeDepth *= farPlane;
		shadow += (currentDepth - bias) > closeDepth ? 1.0f : 0.0f;
	}
	shadow /= samples;
	return shadow;
}

void main()
{
	vec3 objectColor = texture(diffuseMap, fragTexCoords).rgb;
	objectColor = pow(objectColor, vec3(2.2f));
	vec3 lightColor = vec3(0.3f);

	// ambient
	vec3 ambient = vec3(0.1f);
	// diffuse
	vec3 normal = normalize(fragNormal);
	vec3 lightDir = normalize(lightPos - fragPos);
	float diff = max(dot(normal, lightDir), 0);
	vec3 diffuse = diff * lightColor;
	// specular
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 halfVec = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normal, halfVec), 0), 64);
	vec3 specular = spec * lightColor;

	float viewDistance = length(viewDir);
	float bias = 0.15;
	float shadow = calPCFShadow(bias, viewDistance);
	// float shadow = calculateShadow(0.05);
    // float shadow = 0;

	vec3 result = (ambient + (1 - shadow) * (diffuse + specular)) * objectColor;
	result = pow(result, vec3(1.0/2.2));

	fragColor = vec4(result, 1.0f);
}