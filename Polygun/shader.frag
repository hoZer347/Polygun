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
in vec2 ftexCoords;

uniform sampler2D tex;

uniform float Ka = 0.2;
uniform float Kd = 0.7;
uniform float Ks = 0.8;
uniform float shine = 5;
uniform bool do_tex = false;

uniform vec3 lightPos = {100, 100, 100};

uniform bool do_frame = false;

out vec4 outColor;

void main() {
  vec3 N = normalize(fnormal);
  vec3 L = normalize(lightPos - fvertex);

  // Lambert's cosine law
  float lambertian = max(dot(N, L), 0.0);
  float specular = 0.0;
  if(lambertian > 0.0) {
    //vec3 R = reflect(-L, N);
    vec3 R =  -L + 2*(dot(L, N)) * N; // Reflected light vector
    vec3 V = normalize(-fvertex); // Vector to viewer
    // Compute the specular term
    float specAngle = max(dot(R, V), 0.0);
    specular = pow(specAngle, shine);
  }
  outColor = vec4(Ka * vec3(fcolor) + Kd * lambertian * vec3(fcolor) + Ks * specular * vec3(fcolor), 1.0);
  if (do_tex) outColor *= texture2D(tex, ftexCoords);
}
