
#version 410 core


layout(location=0)in vec2 squareVertex;

uniform vec2 Center_worldspace;
uniform vec3 CameraRight_worldspace;
uniform vec3 CameraUp_worldspace;
uniform mat4 VP;

uniform float uStartAngle;

out vec2 UV;

void main()
{
	float angleSpanH = 4.0 * 3.14159265;
	float uWidthSpan = 0.6;
	float startX = -uWidthSpan / 2.0;
	float curAngle = uStartAngle * 1.3 + ((squareVertex.x - startX) / uWidthSpan) * angleSpanH;
	float tz = sin(curAngle) * 0.025;

	float s = 1.0;
	vec3 v = vec3(Center_worldspace, tz)
		+ CameraRight_worldspace *  squareVertex.x * s 
		+ CameraUp_worldspace * squareVertex.y * s;
	gl_Position = VP * vec4(v, 1.0);
	UV = squareVertex + vec2(0.5, 0.5);
}