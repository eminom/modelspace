
#version 330 core

layout(location=0) in vec3 a_position;
layout(location=1) in vec4 a_color;

uniform mat4 u_mvp;

out vec4 fragmentColor;

void main()
{
	gl_Position = u_mvp * vec4(a_position, 1);
	fragmentColor = a_color;
}

