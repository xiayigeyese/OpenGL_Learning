#version 330 core
out vec4 fragColor;

in vec2 fragTexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;
uniform sampler2D blurMap;

struct Light
{
	vec3 position;
	vec3 color;
	float constant;
	float linear;
	float quadratic;
};

uniform Light light;

void main()
{
	vec3 position = texture(gPosition, fragTexCoords).rgb;
	vec3 normal = texture(gNormal, fragTexCoords).rgb;
	vec3 albedo = texture(gAlbedo, fragTexCoords).rgb;
	float occlusion = texture(blurMap, fragTexCoords).r;
	// ambient
	vec3 ambient = 0.3 * albedo * occlusion;
	// diffuse
	vec3 lightDir = normalize(light.position - position);
	float diff = max(dot(lightDir, normal), 0);
	vec3 diffuse = diff * albedo * light.color;
	// specular
	vec3 viewDir = normalize(-position);
	vec3 halfWay = normalize(lightDir + viewDir);
	float spec = pow(max(dot(halfWay, normal), 0), 8.0);
	vec3 specular = spec * light.color;

	vec3 lighting = diffuse + specular;

	// attenuatation
	float dist = distance(position, light.position);
	lighting *=  1.0f / (light.constant + light.linear * dist + light.quadratic * dist * dist);
	// lighting = lighting * vec3(0.00001f) + vec3(0.5f);
	fragColor = vec4(lighting + ambient, 1.0f);
}