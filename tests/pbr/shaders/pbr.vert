#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 fragWorldPos;
out vec3 fragNormal;
out vec2 fragTexCoords;

uniform mat4 u_model;
uniform mat4 u_mvp;

void main()
{
	fragTexCoords = aTexCoords;
	fragWorldPos = vec3(u_model * vec4(aPos, 1.0f));
	fragNormal = normalize(mat3(u_model) * aNormal);
	gl_Position = u_mvp * vec4(aPos, 1.0f);
}
