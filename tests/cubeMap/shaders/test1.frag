#version 330 core

out vec4 FragColor;

in vec3 ourColor;
in vec2 texCoods;

uniform sampler2D texture0;

void main()
{
	FragColor = texture(texture0, texCoods) * vec4(ourColor, 1.0);
	// FragColor = vec4(texture(texture0, texCoods).rgb, 1.0);
	// FragColor = vec4(ourColor, 1.0);
}