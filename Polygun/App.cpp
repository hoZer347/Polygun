#include "App.h"

App::App() {

    // Creating window
    window = glfwCreateWindow(640, 640, "Polygun!!!", NULL, NULL);
    if (!window) {  glfwTerminate(); exit(EXIT_FAILURE); }

    // Making it the focused window for glfw
    glfwMakeContextCurrent(window);

    // Initializing glew
    if (glewInit() != GLEW_OK)
        exit(EXIT_FAILURE);
   
    // Setting the function that happens every frame (NULL for now)
    glfwSetKeyCallback(window, KeyboardCallBack);

    // Setting number of buffers created before rendering
    glfwSwapInterval(1);
}

App::~App() {
    glfwDestroyWindow(window);
}

void App::init() {
    Object o;

    int age = 0;

    // Main game loop
    while (!glfwWindowShouldClose(window)) {
        GLint windowWidth, windowHeight;
        glfwGetWindowSize(window, &windowWidth, &windowHeight);
        glViewport(0, 0, windowWidth, windowHeight);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION_MATRIX);
        glLoadIdentity();

        gluPerspective(60, (double)windowWidth / (double)windowHeight, 0.1, 100);

        glMatrixMode(GL_MODELVIEW_MATRIX);
        glTranslatef(0, 0, -5);

        glRotatef(age, 0, 1, 0);

        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, player.vertices);
        glDrawArrays(GL_QUADS, 0, 24);


        glDisableClientState(GL_VERTEX_ARRAY);

        glfwSwapBuffers(window);
        glfwPollEvents();

        age++;
    }
}

void App::KeyboardCallBack(GLFWwindow* w, int key, int scancode, int action, int mods) {
    switch (key) {
    case GLFW_KEY_W:
        
        break;
    case GLFW_KEY_S:

        break;
    }
}
