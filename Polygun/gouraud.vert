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
uniform bool phong = false;

uniform double x_scale=1;
uniform double y_scale=1;

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
	vec4 vtxPos = view_mat*world_mat*vec4(vertex.x * x_scale, vertex.y * y_scale, vertex.z, 1);
	
	vec4 vtxLight = view_mat*vec4(lightPos,1);
	vec4 vtxNorm = transpose(inverse(view_mat*world_mat))*vec4(normal, 1);
	vec3 ambient, diffuse, specular;
	vec4 vtxColor  = color;

	if (!phong) {
		//the default vtx color. 
		vec3 lighting  = vec3(0,0,0);

		vec3 N = vec3(vtxNorm);
		vec3 V = vec3(vtxPos);

		ambient = coefA * ambient_color;

		vec3 L = normalize(vec3(vtxLight) - V); 
		N = normalize(N);
	
		diffuse = coefD * diffuse_color * max(dot(L, N), 0.f);

		V =  normalize(-V);

		vec3 R = reflect(-L, N);
		specular = coefS * specular_color * pow(max(dot(R, V), 0.f), shine);
		lighting += (do_A*ambient+do_D*diffuse+do_S*specular);
		
		vtxColor.rgb *=lighting;
		fcolor = vtxColor;
		gl_Position = projection_mat*vtxPos;
	} else {
		fcolor = color;
		vec4 vertPos4 = view_mat * vec4(vertex.x*x_scale, vertex.y*y_scale, vertex.z, 1.0);
		vertPos = vec3(vertPos4);
		normalInterp = vec3(world_mat * vec4(normal, 0.0));
		gl_Position =  projection_mat * vertPos4;
	}
}