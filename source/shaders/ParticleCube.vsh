

#version 410 core

layout(location=0)in vec3 a_pos;
layout(location=1)in vec3 a_normal;
layout(location=2)in vec4 a_posfix;
layout(location=3)in vec4 a_color;

uniform mat4 u_ModelMatrix;
uniform mat3 u_NormalMatrix;
uniform vec3 u_LightPosition;
uniform mat4 u_MVP;

out vec4 lightIntensity;
out vec4 fragmentColor;

void main()
{
	vec3 pos_now = a_pos * a_posfix.w + a_posfix.xyz;
	vec4 ic = u_ModelMatrix * vec4(pos_now, 1.0);
	vec3 s = normalize(u_LightPosition - vec3(ic));
	vec3 n = normalize(u_NormalMatrix * a_normal);

	float intensityFactor = length(u_LightPosition - vec3(ic));
	lightIntensity = min(exp(4 / intensityFactor), 256) * vec4(0.003, 0.003, 0.003, 0.05) * max(dot(n, s), 0);
	gl_Position = u_MVP * vec4(pos_now, 1.0);
	fragmentColor = a_color;
}