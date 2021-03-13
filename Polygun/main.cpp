#include "App.h"

int main(void) {
    if (!glfwInit())
        exit(EXIT_FAILURE);

    glewExperimental = true;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    App app;
    app.init();
    
    glfwTerminate();

    exit(EXIT_SUCCESS);

    return 0;
}