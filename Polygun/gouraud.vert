#version 460 core

out vec3 normalInterp;
out vec3 vertPos;

in vec3 vertex;
in vec3 normal;
in vec4 color;

//for gouraud, we only need to send the final vertex color
out vec4 fcolor;

uniform float time;
uniform mat4 MVP;
uniform bool do_frame;

uniform mat4 projection_mat;
uniform mat4 view_mat;
uniform mat4 world_mat;

uniform vec3 lightPos = vec3(240, 210, 200);

uniform vec3 ambient_color =  vec3(1);
uniform vec3 diffuse_color =  vec3(1);
uniform vec3 specular_color = vec3(1);

uniform vec3 shape_color = vec3(1,1,1);

uniform float coefA = 0.2;
uniform float coefD = 0.75;
uniform float coefS = 0.8;

uniform int do_A = 1;
uniform int do_D = 1;
uniform int do_S = 1;

uniform float shine = 5.0f;

void main(){
	vec4 vtxPos = view_mat*world_mat*vec4(vertex, 1);
	
	vec4 vtxLight = view_mat*vec4(lightPos,1);
	vec4 vtxNorm = transpose(inverse(view_mat*world_mat))*vec4(normal, 1);
	vec3 ambient, diffuse, specular;
	vec4 vtxColor  = color;
	fcolor = color;
	vec4 vertPos4 = view_mat * vec4(vertex, 1.0);
	vertPos = vec3(vertPos4);
	normalInterp = vec3(world_mat * vec4(normal, 0.0));
	gl_Position =  projection_mat * vertPos4;
}