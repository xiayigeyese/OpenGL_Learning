#version 330 core
out vec3 fragColor;

in vec2 fragTexCoords;

uniform sampler2D gPositionMap;
uniform sampler2D gNormalMap;
uniform sampler2D ssaoNoiseMap;

uniform vec2 noiseScale;

uniform vec3 samples[64];
uniform int sampleCounts;
uniform float sampleRadius;
uniform float sampleBias;

uniform mat4 projection;

void main()
{
	vec3 viewPos = texture(gPositionMap, fragTexCoords).rgb;
	vec3 viewNormal = texture(gNormalMap, fragTexCoords).rgb;
	vec3 randVec = texture(ssaoNoiseMap, fragTexCoords * noiseScale).rgb;

    // TBN -- Gramm-Schmidt Process
	vec3 tangent = normalize(randVec - viewNormal * dot(randVec, viewNormal));
	vec3 bitangent = cross(viewNormal, tangent);
	mat3 TBN = mat3(tangent, bitangent, viewNormal);

	// cal occlusion
	float occlusion = 0;
	for(int i=0;i<sampleCounts;i++)
	{
		vec3 sampleViewPos = TBN * samples[i];
		sampleViewPos = viewPos + sampleRadius * sampleViewPos;

		// viewSpace -> ndc -> screenSpace
		vec4 samplePos = projection * vec4(sampleViewPos, 1.0f);
		samplePos.xyz = samplePos.xyz / samplePos.w;
		samplePos.xyz = (samplePos.xyz + 1) * 0.5f;

		// pos.z < 0 in viewSpace 
		float sampleDepth = texture(gPositionMap, samplePos.xy).z;

		// range check
		float rangeCheck = smoothstep(0, 1.0f, sampleRadius / abs(viewPos.z - sampleDepth));
		occlusion += (sampleDepth > sampleViewPos.z + sampleBias ? 1.0f : 0.0f) * rangeCheck;		
	}

	fragColor = vec3(1.0 - (occlusion / sampleCounts));
}