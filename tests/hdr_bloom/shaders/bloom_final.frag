#version 330 core
out vec4 fragColor;

in vec2 fragCoords;

uniform sampler2D hdrFBO;
uniform sampler2D blurFBO;
uniform float exposure;

void main()
{
	vec3 hdrColor = texture(hdrFBO, fragCoords).rgb;
	vec3 blurColor = texture(blurFBO, fragCoords).rgb;
	hdrColor += blurColor;
	vec3 result = vec3(1.0f) - exp(-exposure * hdrColor);
	result = pow(result, vec3(1.0 / 2.2));
	fragColor = vec4(result, 1.0f);
}