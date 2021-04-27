#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUVTexCoords;

out vec3 fragPos;
out vec3 fragNormal;
out vec2 fragTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform bool reverseNormal;

void main()
{
	vec4 worldPos = model * vec4(aPos, 1.0f);
	gl_Position = projection * view * worldPos;
	fragPos = worldPos.xyz;
	float reverseFlag = reverseNormal ? -1.0f : 1.0f;
	fragNormal = transpose(inverse(mat3(model))) * (aNormal * reverseFlag);
	fragTexCoords = aUVTexCoords;
}