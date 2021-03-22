#version 460 core

in vec3 vertex;
in vec3 normal;
in vec4 color;

out vec4 fcolor;

uniform mat4 MVP;
uniform bool do_frame;

void main(){
	if (do_frame)	fcolor = vec4(1, 0, 0, 0.9);
	else			fcolor = color;
	gl_Position =  MVP * vec4(vertex, 1);
}