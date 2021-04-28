#version 330 core
out vec4 fragColor;

in vec2 fragTexCoords;

uniform sampler2D fbo;

void main()
{
	vec3 color = texture(fbo, fragTexCoords).rgb;
	vec2 texelSize = 1.0 / textureSize(fbo, 0);
	float weights[5] = float[] (0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162);
	vec3 result = color * weights[0];
	for(int i=1;i<5;i++)
	{
	    // top
		result += weights[i] * texture(fbo, fragTexCoords + vec2(0, i) * texelSize).rgb; 
		// buttom
		result += weights[i] * texture(fbo, fragTexCoords - vec2(0, i) * texelSize).rgb; 
	}
	fragColor = vec4(result, 1.0f);
}