

#version 330 core

out vec4 fragmentColor;
layout(location=0)in vec3 a_position;
layout(location=1)in vec4 a_color;
layout(location=2)in vec3 a_fix;

uniform mat4 u_vp;
uniform mat4 u_m;
uniform mat4 u_s;	

void main()
{
	mat4 matx = mat4(
		vec4(1,0,0,0),
		vec4(0,1,0,0),
		vec4(0,0,1,0),
		vec4(a_fix[0],a_fix[1],a_fix[2], 0)
	);

	// The add applied. 
	matx += u_m;
	vec4 pos = u_vp * matx * u_s * vec4(a_position, 1);
	gl_Position = pos;
	fragmentColor = a_color;
}

