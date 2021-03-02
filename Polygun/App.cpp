#include "App.h"

App::App() {
    // Creating window
    window = glfwCreateWindow(640, 640, "Polygun!!!", NULL, NULL);
    if (!window) {  glfwTerminate(); exit(EXIT_FAILURE); }

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Making it the focused window for glfw
    glfwMakeContextCurrent(window);

    // Initializing glew
    if (glewInit() != GLEW_OK)
        exit(EXIT_FAILURE);
   
    // Setting the functions that happens every input
    glfwSetKeyCallback(window, KeyboardCallBack);

    // Setting number of buffers created before rendering
    glfwSwapInterval(1);

    objects.push_back(&player);

    // Testing stuff to be removed
    Object* o = new Object();
    Cube* cube = new Cube("test");
    *cube -= glm::vec3(1, 1.1, 0);
    o->geometry.push_back(cube);

    objects.push_back(o);
    o = new Object();

    o->geometry.push_back(new Plane("plane"));
    objects.push_back(o);
    //
}

App::~App() {
    glfwDestroyWindow(window);
}

void App::mouse() {
    glfwGetCursorPos(window, &mx, &my);
    if (my > 90*4) glfwSetCursorPos(window, mx, 90*4);
    else if (my < -90*4) glfwSetCursorPos(window, mx, -90*4);
}

void App::init() {
    int age = 0;

    // Main game loop
    while (!glfwWindowShouldClose(window)) {
        double start_time = glfwGetTime();

        GLint windowWidth, windowHeight;
        glfwGetWindowSize(window, &windowWidth, &windowHeight);
        glViewport(0, 0, windowWidth, windowHeight);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION_MATRIX);
        glLoadIdentity();

        gluPerspective(60, (double)windowWidth / (double)windowHeight, 0.1, 100);

        glMatrixMode(GL_MODELVIEW_MATRIX);
        mouse();
        cam.rotate(glm::vec3(0, mx/4, my/4));
        cam.update(window);

        for (auto& o : objects)
            o->render(window);

        glfwSwapBuffers(window);
        glfwPollEvents();

        age++;

        if (glfwGetKey(window, GLFW_KEY_SPACE))         cam += glm::vec3( 0,   -0.1,  0   );
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT))    cam += glm::vec3( 0,    0.1,  0   );

        if (glfwGetKey(window, GLFW_KEY_W))             cam += glm::vec3( 0,    0,    0.1 );
        if (glfwGetKey(window, GLFW_KEY_A))             cam += glm::vec3( 0.1,  0,    0   );
        if (glfwGetKey(window, GLFW_KEY_S))             cam += glm::vec3( 0,    0,   -0.1 );
        if (glfwGetKey(window, GLFW_KEY_D))             cam += glm::vec3(-0.1,  0,    0   );
        
        double end_time = glfwGetTime();

        while (end_time - start_time < 1/60)
            end_time = glfwGetTime();
    }
}

void App::KeyboardCallBack(GLFWwindow* w, int key, int scancode, int action, int mods) {
    switch (key) {
    case GLFW_KEY_ESCAPE:
        exit(0);
        break;
    case GLFW_KEY_TAB:
        auto monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);

        glfwSetWindowMonitor(w, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
        break;
    }
}
