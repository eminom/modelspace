
#version 330 core

in vec4 fragmentColor;
in vec4 lightIntensity;
layout(location=0)out vec4 color;

void main()
{
	//color = fragmentColor * lightIntensity;
	vec4 colorOut = fragmentColor;
	colorOut.rgb *= 0.2;
	color = lightIntensity + colorOut;
	// color = fragmentColor;
}