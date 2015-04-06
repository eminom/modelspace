

#version 410 core

in vec4 lightIntensity;
in vec4 fragmentColor;
out vec4 color;

void main()
{
	vec4 nxcolor = fragmentColor;
	nxcolor += 0.2 * lightIntensity;
	color = nxcolor;
}
