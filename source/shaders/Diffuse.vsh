
#version 330 core

layout(location=0)in vec3 a_position;
layout(location=1)in vec3 a_normal;
layout(location=2)in vec4 a_color;

out vec4 lightIntensity;
out vec4 fragmentColor;

uniform vec3 u_LightPosition;
uniform mat4 u_ModelMatrix;
uniform mat3 u_NormalMatrix;
uniform mat4 u_MVP;

void main()
{
	vec4 ic = u_ModelMatrix * vec4(a_position, 1);
	vec3 s  = normalize(u_LightPosition - vec3(ic));
	vec3 n  = normalize(u_NormalMatrix * a_normal);

	float intensityFactor = length(u_LightPosition - vec3(ic));

	lightIntensity = min(exp(4 / intensityFactor), 256) * vec4(0.003, 0.003, 0.003, 0.05) * max(dot(n, s), 0);
	fragmentColor = a_color;
	gl_Position = u_MVP * vec4(a_position, 1);
}