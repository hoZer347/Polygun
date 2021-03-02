#include "Cube.h"

void Cube::load() {
    vertices = {
        0, 0, 0,  0, 0, 1,  0, 1, 1,  0, 1, 0,
        1, 0, 0,  1, 0, 1,  1, 1, 1,  1, 1, 0,
        0, 0, 0,  0, 0, 1,  1, 0, 1,  1, 0, 0,
        0, 1, 0,  0, 1, 1,  1, 1, 1,  1, 1, 0,
        0, 0, 0,  0, 1, 0,  1, 1, 0,  1, 0, 0,
        0, 0, 1,  0, 1, 1,  1, 1, 1,  1, 0, 1
    };
}

void Cube::render(GLFWwindow* window) {
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices.data());
    glColor3f(1, 1, 1);
    glDrawArrays(GL_QUADS, 0, 24);
    glColor3f(1, 0, 0);
    glDrawArrays(GL_LINE_LOOP, 0, 24);

    glDisableClientState(GL_VERTEX_ARRAY);
}
