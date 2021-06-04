#version 330 core
in vec4 fragPos;

uniform float farPlane;
uniform vec3 ligthPos;

void main()
{
	float dist = length(fragPos.xyz - ligthPos);
	dist = dist / farPlane;
	gl_FragDepth = dist;
}