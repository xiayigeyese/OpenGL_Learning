#version 330 core
out vec4 fragColor;

in vec2 fragTexCoords;

uniform sampler2D u_texture;

void main()
{
	fragColor = vec4(texture(u_texture, fragTexCoords).rg, 0.0, 1.0f);
}