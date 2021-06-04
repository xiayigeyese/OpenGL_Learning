#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 fragPos;
out vec3 fragNormal;
out vec2 fragTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	vec4 viewPos = view * model * vec4(aPos, 1.0f);
	
	fragPos = viewPos.xyz;
	fragNormal = transpose(inverse(mat3(view * model))) * aNormal;
	fragTexCoords = aTexCoords;

	gl_Position = projection * viewPos;
}