

#version 410 core

//in vec2 uv;
in vec4 particlecolor;
out vec4 color;
//uniform sampler2D id;

void main(){
	// Output color = color of the texture at the specified UV
	//color = texture2D( id, UV ) * particlecolor;
	color = particlecolor;
}