#include "App.h"

App::App() {

    // Creating window
    window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
    if (!window) {  glfwTerminate(); exit(EXIT_FAILURE); }

    // Making it the focused window for glfw
    glfwMakeContextCurrent(window);

    // Initializing glew
    if (glewInit() != GLEW_OK)
        exit(EXIT_FAILURE);
   
    // Setting the function that happens every frame (NULL for now)
    glfwSetKeyCallback(window, NULL);

    // Setting number of buffers created before rendering
    glfwSwapInterval(1);
}

App::~App() {
    glfwDestroyWindow(window);
}

void App::init() {

    // Main game loop
    while (!glfwWindowShouldClose(window)) {
        glfwSwapBuffers(window);


        glfwPollEvents();
    }
}
