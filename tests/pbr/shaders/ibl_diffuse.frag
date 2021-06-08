#version 330 core
out vec4 fragColor;

in vec3 fragWorldPos;
in vec3 fragNormal;
in vec2 fragTexCoords;

// light
struct Light
{
	vec3 position;
	vec3 color;
};

// material parameter
uniform vec3 albedo;
uniform float metallic;
uniform float roughness;
uniform float ao;
uniform samplerCube u_irradianceMap;

// other
uniform Light light[4];
uniform vec3 viewPos;


// ======= BRDF =======

const float PI = 3.14159265359;

float D_GGX_TR(vec3 n, vec3 h, float alpha)
{
	float a = alpha * alpha;
	float b = max(dot(n, h), 0);
	float c = b * b * (a - 1) + 1;
	return a / (PI * c * c);
}

float G_Schlick_GGX(vec3 n, vec3 v, float k)
{
	float a = max(dot(n, v), 0);
	return a / (a * (1 - k) + k);
}

float G_Smith_direct(vec3 n, vec3 v, vec3 l, float roughness)
{
	float k = (roughness + 1) * (roughness + 1) / 8.0f;
	return G_Schlick_GGX(n, v, k) * G_Schlick_GGX(n, l, k);
}

vec3 F_Fresnel_Schlick(float cosTheta, vec3 F0)
{
	return F0 + (1.0 - F0) * pow(1 - cosTheta, 5.0f);
}


void main()
{
	vec3 V = normalize(viewPos - fragWorldPos);
	vec3 N = fragNormal;
	vec3 F0 = mix(vec3(0.04f), albedo, metallic);

	vec3 outRadiance = vec3(0.0);
	for (int i=0; i<4; i++)
	{
		// in radiance
		float dist = distance(light[i].position, fragWorldPos);
		float attenuation = 1.0f / (dist * dist);
		vec3 radiance = light[i].color * attenuation;

		// BRDF -- ks * f_specular
		vec3 L = normalize(light[i].position - fragWorldPos);
		vec3 H = normalize(L + V);
		float D = D_GGX_TR(N, H, roughness * roughness);
		vec3 F = F_Fresnel_Schlick(clamp(dot(H, V), 0.0, 1.0), F0);
		float G = G_Smith_direct(N, V, L, roughness);
		float denominator = 4 * max(dot(N, L), 0.0) * max(dot(N, V), 0.0);
		// avoid divide zero
		vec3 fSpecular = D * F * G / max(denominator, 0.001);

		// BRDF -- kd * f_lambert
		vec3 ks = F;
		vec3 kd = (1 - ks) * (1 - metallic);
		vec3 fLambert = kd * albedo / PI;

		// BRDF 
		vec3 brdf = fLambert + fSpecular;

		// out radiance
		outRadiance += brdf * radiance * max(dot(N, L), 0);
	}

	// ambient -- IBL £º diffuse
	vec3 irrandiance = texture(u_irradianceMap, N).rgb;
	vec3 ks = F_Fresnel_Schlick(clamp(dot(N, V), 0.0, 1.0), F0);
	vec3 kd = (1 - ks) * (1 - metallic);
	vec3 diffuse = albedo * irrandiance;  // irrandiance is divise PI when calculate
	vec3 ambient = kd * diffuse * ao;
	// vec3 ambient = vec3(0.03) * albedo * ao;

	// light color
	vec3 color = ambient + outRadiance;
	// vec3 color = ambient;

	// tone mapping
	color = color / (color + vec3(1.0f));
	// gamma correct
	color = pow(color, vec3(1.0f/2.2f));

	fragColor = vec4(color, 1.0f);
}