
#version 410 core
in vec2 uv;
out vec3 color;

uniform sampler2D tex;
uniform float time;

void main(){
	color = texture(tex, uv + 0.005*vec2(sin(time+1024.0*uv.x), cos(time+768.0*uv.y)) ).xyz;
}