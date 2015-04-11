

#version 410 core

in vec2 UV;
in vec4 particlecolor;
out vec4 color;
uniform sampler2D id;

void main(){
	// Output color = color of the texture at the specified UV
	color = texture( id, UV );// * particlecolor;
	// color = particlecolor;
}