#version 330 core
out vec4 fragColor;

in vec2 fragTexCoords;

uniform sampler2D hdrFbo;
uniform float exposure;

void main()
{
	vec3 hdrColor = texture(hdrFbo, fragTexCoords).rgb;
	// exposure
    vec3 result = vec3(1.0) - exp(-hdrColor * exposure);
    result = pow(result, vec3(1.0 / 2.2));
	fragColor = vec4(result, 1.0f);
}