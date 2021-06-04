#version 330 core
layout (location = 0) out vec4 fragColor;
layout (location = 1) out vec4 prevBlurColor;

uniform vec3 lightColor;

void main()
{
	vec3 result = lightColor;
	// out fragColor 
	fragColor = vec4(result, 1.0f);
    // out lightColor for blur
    float brightness = dot(result, vec3(0.2126, 0.7152, 0.0722));
	if (brightness>1.0f) 
	{
		prevBlurColor = vec4(result, 1.0f);
	}
	else 
	{
	    prevBlurColor = vec4(0,0,0,1.0);
	}
}