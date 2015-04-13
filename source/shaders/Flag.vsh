
#version 410 core


layout(location=0)in vec2 squareVertex;

uniform vec2 Center_worldspace;
uniform vec3 CameraRight_worldspace;
uniform vec3 CameraUp_worldspace;
uniform mat4 VP;

out vec2 UV;

void main()
{
	float s = 1.0;
	vec3 v = vec3(Center_worldspace, 0)
		+ CameraRight_worldspace *  squareVertex.x * s 
		+ CameraUp_worldspace * squareVertex.y * s;
	gl_Position = VP * vec4(v, 1.0);
	UV = squareVertex + vec2(0.5, 0.5);
}