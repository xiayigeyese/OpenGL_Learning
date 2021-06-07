#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoods;

out vec3 fragPos;
out vec3 fragNormal;
out vec2 fragTexCoords;

uniform mat4 u_model;
uniform mat4 u_mvp;

void main()
{
	fragPos = (u_model * vec4(aPos, 1.0f)).xyz;
	fragTexCoords = aTexCoods;
	fragNormal = normalize(mat3(u_model) * aNormal);
	gl_Position = u_mvp * vec4(aPos, 1.0f);
}