#include "App.h"

App::App() {
	// Generating window
	window = glfwCreateWindow(1024, 768, "Polygun", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSetWindowUserPointer(window, this);
	glfwSetCursorPos(window, 0, 0);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Enabling openGL to do stuff automatically
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glAlphaFunc(GL_GREATER, 0.5);
	glEnable(GL_ALPHA_TEST);

	glewExperimental = true;

	// Starting glew
	if (glewInit() != GLEW_OK)
		exit(0);

	// Assigning callback functions
	glfwSetCursorPosCallback(window, CursorCallback);
	glfwSetKeyCallback(window, KeyPrsCallback);
	glfwSetMouseButtonCallback(window, MouseBCallback);
	glfwSetWindowSizeCallback(window, ResizeCallback);
	glfwSetScrollCallback(window, ScrollCallback);

	Vtx v1;
	Vtx v2;
	Vtx v3;
	Vtx v4;

	v1.load({ 0, 0, 0 });
	v2.load({ 0, 1, 0 });
	v3.load({ 0, 1, 1 });
}

App::~App() {
	
}

void App::test() {

}

void App::init() {
	// Setting screen clear color
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// Shader stuff
	shader = LS("shader");
	mvpID = glGetUniformLocation(shader, "MVP");
	clrID = glGetUniformLocation(shader, "fcolor");
	glUseProgram(shader);
	//

	// Generating vertex array
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	//

	// Generating buffers
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glGenBuffers(1, &cbo);
	glBindBuffer(GL_COLOR_ARRAY, cbo);
	//

	glm::mat4x4 Projection = glm::perspective(60.0f, 4.0f / 3.0f, 0.1f, 100.0f);

	glm::mat4x4 View = glm::lookAt(
		glm::vec3(5, 0, 0),
		glm::vec3(0, 0, 0),
		glm::vec3(0, 1, 0)
	);

	glm::mat4x4 Model = glm::mat4(1.0f);

	glm::mat4x4 MVP = Projection * View * Model;

	glUniformMatrix4fv(mvpID, 1, GL_FALSE, &MVP[0][0]);

	do {
		// Starting fps timer
		double start_time = glfwGetTime();

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Pumping vertices into the vertex buffer
		pump(VERTICES, GL_TRIANGLES);
		pump(FRAME, GL_LINE_LOOP);
		//

		// End of loop stuff
		glfwSwapBuffers(window);
		glfwPollEvents();
		do_inputs();
		//

		// Capping fps
		double end_time = glfwGetTime();
		double elapsed_time = end_time - start_time;

		while (end_time - start_time < 1 / 60)
			end_time = glfwGetTime();
		//

		// Testing if the window should close
	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

	// Deallocating memory
	glDeleteBuffers(1, &vbo);
	glDeleteProgram(shader);
	glDeleteVertexArrays(1, &vao);
	glfwDestroyWindow(window);
	glfwTerminate();
	//

	return;
}

void App::pump(std::vector<GLfloat>& v, GLenum type) {
	int size = v.size();

	//glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * size, v.data(), GL_DYNAMIC_DRAW);

	//glEnableVertexAttribArray(0);
	//glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	switch (type) {
	case GL_TRIANGLES:
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * size, v.data(), GL_DYNAMIC_DRAW);
		glBufferData(GL_COLOR_ARRAY, sizeof(GLfloat) * COLOR.size(), COLOR.data(), GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		glEnableVertexAttribArray(vbo);
		glVertexAttribPointer(vbo, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glEnableVertexAttribArray(cbo);
		glVertexAttribPointer(cbo, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glUniform4f(clrID, 1, 1, 1, 1);

		glDrawArrays(type, 0, size);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		break;
	case GL_LINE_LOOP:
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glUniform4f(clrID, 0, 0, 0, 1);
		for (int i = 0; i < size/2; i++)
			glDrawArrays(type, i*2, 2);

		glDisableVertexAttribArray(0);
	}
}

// WASD Movement, SPACE is up, LCONTROL = down
void App::do_inputs() {
	App* app = (App*)glfwGetWindowUserPointer(window);
}
void App::CursorCallback(GLFWwindow* window, double xpos, double ypos) {
	App* app = (App*)glfwGetWindowUserPointer(window);
}
void App::KeyPrsCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	App* app = (App*)glfwGetWindowUserPointer(window);
}
void App::MouseBCallback(GLFWwindow* window, int button, int action, int mods) {
	App* app = (App*)glfwGetWindowUserPointer(window);
}
void App::ResizeCallback(GLFWwindow* window, int width, int height) {
	App* app = (App*)glfwGetWindowUserPointer(window);
}
void App::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	App* app = (App*)glfwGetWindowUserPointer(window);
}
