#include "App.h"

App::App() {
	window = glfwCreateWindow(1024, 768, "Polygun", NULL, NULL);

	glfwMakeContextCurrent(window);

	// Enabling openGL to do stuff
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glAlphaFunc(GL_GREATER, 0.5);
	glEnable(GL_ALPHA_TEST);

	glewExperimental = true;

	if (glewInit() != GLEW_OK)
		exit(0);

	glfwSetWindowUserPointer(window, this);

	glfwSetCursorPosCallback(window, CursorCallback);
	glfwSetKeyCallback(window, KeyPrsCallback);
	glfwSetMouseButtonCallback(window, MouseBCallback);
	glfwSetWindowSizeCallback(window, ResizeCallback);
	glfwSetScrollCallback(window, ScrollCallback);

	// Testing objects
	Object* o = new Object();
	Cube* cube = new Cube();
	*cube += glm::vec3(-0.5, -0.5, -0.5);
	o->geo.push_back(cube);
	objects.push_back(o);
	//
}

App::~App() {

}

void App::init() {
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	shader = LS("shader");

	mvpID = glGetUniformLocation(shader, "MVP");
	clrID = glGetUniformLocation(shader, "fcolor");

	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);

	glfwSetCursorPos(window, 0, 0);

	do {
		double start_time = glfwGetTime();

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Pumping vertices into vertex buffer
		for (auto& o : objects)
			for (auto& g : o->geo) {
				vertices.insert(vertices.end(), g->vertices.begin(), g->vertices.end());
				frame.insert(frame.end(), g->frame.begin(), g->frame.end());
			}

		cam.Projection = glm::perspective(cam.FoV, cam.aspectRatio, 0.1f, cam.renderDistance);

		pump(vertices, GL_TRIANGLES);
		pump(frame, GL_LINE_LOOP);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
		do_inputs();

		double end_time = glfwGetTime();

		while (end_time - start_time < 1 / 60)
			end_time = glfwGetTime();

	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	// Cleanup VBO and shader
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteProgram(shader);
	glDeleteVertexArrays(1, &VertexArrayID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return;
}

void App::pump(std::vector<GLfloat> v, GLenum type) {
	glUseProgram(shader);

	int size = v.size();

	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * size, v.data(), GL_STATIC_DRAW);

	glUniformMatrix4fv(mvpID, 1, GL_FALSE, &cam.update()[0][0]);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
	);

	switch (type) {
	case GL_TRIANGLES:
		glUniform4f(clrID, 1, 1, 1, 1);
		glDrawArrays(type, 0, size);
		break;
	case GL_LINE_LOOP:
		glUniform4f(clrID, 0, 0, 0, 1);
		for (int i = 0; i < size/2; i++)
			glDrawArrays(type, i*2, 2);
	}

	glDisableVertexAttribArray(0);

	v.clear();
}

void App::do_inputs() {
	if (glfwGetKey(window, GLFW_KEY_W)) cam += glm::vec3(0, 0, -0.1);
	if (glfwGetKey(window, GLFW_KEY_A)) cam += glm::vec3(-0.1, 0, 0);
	if (glfwGetKey(window, GLFW_KEY_S)) cam += glm::vec3(0, 0, 0.1);
	if (glfwGetKey(window, GLFW_KEY_D)) cam += glm::vec3(0.1, 0, 0);
}

void App::CursorCallback(GLFWwindow* window, double xpos, double ypos) {
	App* app = (App*)glfwGetWindowUserPointer(window);

	app->cam.rotate(glm::vec3(0, xpos/4, ypos/4));
}
void App::KeyPrsCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	App* app = (App*)glfwGetWindowUserPointer(window);

	switch (key) {
	case GLFW_KEY_TAB:
		auto monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);

		glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);

		break;
	}
}
void App::MouseBCallback(GLFWwindow* window, int button, int action, int mods) {
	App* app = (App*)glfwGetWindowUserPointer(window);
}
void App::ResizeCallback(GLFWwindow* window, int width, int height) {
	App* app = (App*)glfwGetWindowUserPointer(window);

	GLint windowWidth, windowHeight;
	glfwGetWindowSize(window, &windowWidth, &windowHeight);
	glViewport(0, 0, windowWidth, windowHeight);

	app->cam.aspectRatio = (float)windowWidth / (float)windowHeight;
}
void App::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	App* app = (App*)glfwGetWindowUserPointer(window);
}
