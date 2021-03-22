#version 460 core

in vec3 normalInterp;
in vec3 vertPos;

in vec4 fcolor;

out vec4 outColor;

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

void main() {
    if (phong) {
        vec3 N = normalize(normalInterp);
        vec3 L = normalize(lightPos - vertPos);

        // Lambert's cosine law
        float lambertian = max(dot(N, L), 0.0);
        float specular = 0.0;
        if(lambertian > 0.0) {
            vec3 R = reflect(-L, N);      // Reflected light vector
            vec3 V = normalize(-vertPos); // Vector to viewer
            // Compute the specular term
            float specAngle = max(dot(R, V), 0.0);
            specular = pow(specAngle, shine);
        }
        outColor = fcolor * vec4(
            do_A * coefA * ambient_color +
            do_D * coefD * lambertian * diffuse_color +
            do_S * coefS * specular * specular_color, 1.0);
    } else {
        outColor = fcolor;
    }
}