#version 330

layout(location = 0) in vec3 vertexPosition_modelspace;

uniform mat4 MVP;
uniform vec4 normal;
uniform mat4 view_mat;
uniform vec3 shape_color = vec3(0.5,1.0,0.2);
uniform vec3 lightPos = vec3(4.0,0,5);

//the colors that the light shines on for the modal 
uniform vec3 ambient_color =  vec3(1);
uniform vec3 diffuse_color =  vec3(1);
uniform vec3 specular_color = vec3(1);

uniform float coefA =0.1;
uniform float coefD = 1;
uniform float coefS = 1.5;
uniform float shine = 32.0f;

out vec4 fColor;

void main(){
//the default vtx color. 
	vec4 vtxColor  = vec4(shape_color,1.0);

	//For the position, light source and normals, we need to convert them to view space
	vec4 vtxPos =  MVP * vec4(vertexPosition_modelspace,1);
	vec4 vtxLight = vec4(lightPos,1);
	vec4 vtxNorm = normal;

	vec3 N = vec3(vtxNorm);
	vec3 V = vec3(vtxPos);

	vec3 ambient, ambient2, diffuse, diffuse2, specular;
	
	//ambient----------------------------------------
	ambient = coefA * ambient_color;

	//diffuse----------------------------------------
	vec3 L = normalize(vec3(vtxLight) - V); 
	N = normalize(N);

	
	diffuse = coefD * diffuse_color * max(dot(L, N), 0.f);
	
	//specular----------------------------------------

	//because we're in view space, everything is relative to the camera, so no need to 
	//subtract the vertex position from the camera position
	V =  normalize(-V);

	//You can also use the reflect function for computing the reflection direction:
    //vec3 R = reflect(-L, N);
	vec3 R=  -L+2*dot(L, N)*N;
	specular = coefS * specular_color * pow(max(dot(R, V), 0.f), shine);
	////apply 
	vtxColor.rgb *= (ambient+diffuse+specular);
	fColor = vtxColor;

	gl_Position =  MVP * vec4(vertexPosition_modelspace,1);

}