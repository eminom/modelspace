
#version 330 core

layout(location=0)in vec3 a_position;
layout(location=1)in vec3 a_normal;
layout(location=2)in vec4 a_color;

out vec4 lightIntensity;
out vec4 fragmentColor;

uniform vec3 u_LightPosition0;
uniform vec3 u_LightPosition1;
uniform mat4 u_ModelMatrix;
uniform mat3 u_NormalMatrix;
uniform mat4 u_MVP;

void main()
{
	vec4 ic = u_ModelMatrix * vec4(a_position, 1);
	vec3 n  = normalize(u_NormalMatrix * a_normal);
	float intensityFactor;
	vec3 s;
	vec4 intense, intense1;

	intensityFactor = length(u_LightPosition0 - vec3(ic));
	s = normalize(u_LightPosition0 - vec3(ic));
	intense = min(exp(4 / intensityFactor), 256) * vec4(0.003, 0.003, 0.003, 0.05) * max(dot(n, s), 0);

	intensityFactor = length(u_LightPosition1- vec3(ic));
	s = normalize(u_LightPosition1- vec3(ic));
	intense1= min(exp(4 / intensityFactor), 256) * vec4(0.003, 0.003, 0.003, 0.05) * max(dot(n, s), 0); 

	lightIntensity = intense + intense1;
	fragmentColor = a_color;
	gl_Position = u_MVP * vec4(a_position, 1);
}