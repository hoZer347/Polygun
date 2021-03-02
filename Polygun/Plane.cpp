#include "Plane.h"

void Plane::load() {
    vertices = {
        -1000, -100, -1000,
         1000, -100, -1000,
         1000, -100,  1000,
        -1000, -100,  1000
    };
}

void Plane::render() {
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices.data());
    glColor3f(0, 0, 1);
    glDrawArrays(GL_QUADS, 0, 4);

    glDisableClientState(GL_VERTEX_ARRAY);
}