#version 460 core

//in vec3 vertex;
//in vec3 normal;
//in vec4 color;
//
//out vec4 fcolor;
//
//uniform mat4 MVP;
//uniform bool do_frame;
//
//void main(){
//	if (do_frame)	fcolor = vec4(1, 0, 0, 0.9);
//	else			fcolor = color;
//	gl_Position =  MVP * vec4(vertex, 1);
//}

in vec3 vertex;
in vec3 normal;
in vec4 color;
in vec2 texCoords;

uniform mat4 mode, view, proj, MVP;
uniform bool do_frame = false;

out vec3 fnormal;
out vec3 fvertex;
out vec4 fcolor;
out vec2 ftexCoords;

void main() {
		ftexCoords = texCoords;

		if (do_frame) fcolor = vec4(0.5f, 0, 0, 1);
		else fcolor = color;

		vec4 vertPos4 = view * vec4(vertex, 1.0);
		fvertex = vec3(vertPos4);
		fnormal = vec3(mode * vec4(normal, 0.0));

		gl_Position =  MVP * vec4(vertex, 1);
}
