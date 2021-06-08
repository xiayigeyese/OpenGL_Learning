#version 330 core
out vec4 fragColor;

in vec3 fragWorldPos;
in vec3 fragNormal;
in vec2 fragTexCoords;

uniform vec3 u_albedo;
uniform float u_metallic;
uniform float u_roughness;
uniform float u_ao;

uniform samplerCube u_irradianceMap;
uniform samplerCube u_prefilterMap;
uniform sampler2D u_brdfLUT;

uniform float u_maxPrefilterMapMipLevel;

struct Light
{
	vec3 position;
	vec3 color;
};

uniform Light u_lights[4];
uniform vec3 u_viewPos;

const float PI = 3.14159265359f;


float D_GGX_TR(float NdotH, float alpha);
float G_Schlick_GGX(float cosTheta, float k);
float G_Smith_direct(float NdotL, float NdotV, float roughness);
float G_Smith_IBL(float NdotL, float NdotV, float roughness);
vec3 F_Fresnel_Schlick(float VdotH, vec3 F0);
vec3 FresnelSchlickRoughness(float VdotH, vec3 F0, float roughness);

void main()
{
	vec3 N = fragNormal;
	vec3 V = normalize(u_viewPos - fragWorldPos);
	vec3 R = reflect(-V, N);
	float NdotV = max(dot(N, V), 0);
	vec3 F0 = mix(vec3(0.04f), u_albedo, u_metallic);

	// direct lights
	vec3 outRadiance = vec3(0.0);
	for(int i=0;i<4;i++)
	{
		// in radiance
		float dist = distance(u_lights[i].position, fragWorldPos);
		float attenuation = 1.0f / (dist * dist);
		vec3 radiance = u_lights[i].color * attenuation;

		// BRDF
		vec3 L = normalize(u_lights[i].position - fragWorldPos);
		vec3 H = normalize(V + L);
		float NdotL = max(dot(N, L), 0);
		float NdotH = max(dot(N, H), 0);
		float VdotH = max(dot(V, H), 0);
		// BRDF --> ks * f_spcular
		float D = D_GGX_TR(NdotH, u_roughness * u_roughness);
		float G = G_Smith_direct(NdotL, NdotV, u_roughness);
		vec3 F = F_Fresnel_Schlick(VdotH, F0);
		float denominator = 4 * NdotL * NdotV;
		vec3 fSpecular = D * F * G / max(denominator, 0.001);
        // BRDF --> kd * f_lambert
		vec3 ks = F;
		vec3 kd = (1-ks) * (1-u_metallic);
		vec3 fLambert = kd * u_albedo / PI;
        // BRDF
		vec3 brdf = fLambert + fSpecular;

		// out radiance
		outRadiance += radiance * brdf * NdotL;
	}

	// ambient -- IBL
	vec3 F = FresnelSchlickRoughness(NdotV, F0, u_roughness);
	// diffuse
	vec3 irrandiance = texture(u_irradianceMap, N).rgb;
	vec3 ks =  F;
	vec3 kd = (1 - ks) * (1 - u_metallic);
	vec3 ibl_diffuse = u_albedo * irrandiance; 

	// specular
	float mipLevel = u_roughness * (u_maxPrefilterMapMipLevel - 1);
	vec3 prefilteredColor = textureLod(u_prefilterMap, R, mipLevel).rgb;
	vec2 envBRDF = texture(u_brdfLUT, vec2(NdotV, u_roughness)).rg;
	vec3 ibl_specular = prefilteredColor * (F * envBRDF.x + envBRDF.y);

	vec3 ambient  = (kd * ibl_diffuse  + ibl_specular) * u_ao;

	// lightColor
	vec3 color = ambient + outRadiance;

	// tone mappint + gamma correct
	color = color / (color + vec3(1.0f));
	color = pow(color, vec3(1.0 / 2.2));

	fragColor = vec4(color, 1.0f);
}

float D_GGX_TR(float NdotH, float alpha)
{
	float a = alpha * alpha;
	float b = NdotH;
	float c = b * b * (a - 1) + 1;
	return a / (PI * c * c);
}

float G_Schlick_GGX(float cosTheta, float k)
{
	return cosTheta / (cosTheta * (1 - k) + k);
}

float G_Smith_direct(float NdotL, float NdotV, float roughness)
{
	float k = (roughness + 1) * (roughness + 1) / 8.0f;
	return G_Schlick_GGX(NdotL, k) * G_Schlick_GGX(NdotV, k);
}

float G_Smith_IBL(float NdotL, float NdotV, float roughness)
{
	float k = roughness * roughness / 2.0f;
	return G_Schlick_GGX(NdotL, k) * G_Schlick_GGX(NdotV, k);
}

vec3  F_Fresnel_Schlick(float VdotH, vec3 F0)
{
	return F0 + (1.0 - F0) * pow(1 - VdotH, 5.0f);
}

vec3 FresnelSchlickRoughness(float VdotH, vec3 F0, float roughness)
{
	return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - VdotH, 5.0);
}