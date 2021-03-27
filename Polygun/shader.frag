#version 460 core

//in vec4 fcolor;
//
//out vec4 color;
//
//void main() {
//	color = fcolor;
//}

in vec3 fnormal;
in vec3 fvertex;
in vec4 fcolor;

uniform float Ka = 0.2;
uniform float Kd = 0.7;
uniform float Ks = 0.8;
uniform float shine = 5;

uniform vec3 ambient_color = {1, 1, 1};
uniform vec3 diffuse_color = {1, 1, 1};
uniform vec3 specular_color = {1, 1, 1};
uniform vec3 lightPos = {100, 100, 100};

uniform bool do_frame = false;

out vec4 outColor;

void main() {
    if (do_frame) { outColor = fcolor; return; }

    vec3 N = normalize(fnormal);
    vec3 L = normalize(lightPos - fvertex);

    // Lambert's cosine law
    float lambertian = max(dot(N, L), 0.0);
    float specular = 0.0;
    if(lambertian > 0.0) {
        vec3 halfwayDir = normalize(L + normalize(-fvertex));
        specular = pow(max(dot(N, halfwayDir), 0.0), shine);
    }
    outColor = fcolor * vec4(
        Ka * ambient_color +
        Kd * lambertian * diffuse_color +
        Ks * specular * specular_color, 1.0);
}
