
#version 410 core

uniform sampler2D u_ID;
in vec2 UV;
out vec4 color;

void main()
{
	color = texture(u_ID, vec2(UV.x, 1-UV.y));
}