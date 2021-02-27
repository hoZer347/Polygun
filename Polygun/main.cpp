// Polygun uwu

#include "App.h"

int main(void) {
    if (!glfwInit())
        exit(EXIT_FAILURE);

    App app;
    app.init();
    
    glfwTerminate();
    exit(EXIT_SUCCESS);

    return 0;
}