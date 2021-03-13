#include "App.h"

App::App() {
	// Generating window
	window = glfwCreateWindow(1024, 768, "Polygun", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSetWindowUserPointer(window, this);
	glfwSetCursorPos(window, 0, 0);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//

	// Anti-aliasing
	glEnable(GL_MULTISAMPLE);
	glfwWindowHint(GLFW_SAMPLES, 4);
	//
	
	// Blocking stuff that are behind opaque objects
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	//

	// Alpha processing setup
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glAlphaFunc(GL_GREATER, 0.5);
	glEnable(GL_ALPHA_TEST);
	//

	// Culling faces that don't face the camera
	glEnable(GL_CULL_FACE);

	// Starting glew
	if (glewInit() != GLEW_OK)
		exit(0);

	// Assigning callback functions
	glfwSetCursorPosCallback(window, CursorCallback);
	glfwSetKeyCallback(window, KeyPrsCallback);
	glfwSetMouseButtonCallback(window, MouseBCallback);
	glfwSetWindowSizeCallback(window, ResizeCallback);
	glfwSetScrollCallback(window, ScrollCallback);
	//
}

App::~App() {
	
}

void App::init() {
	// Setting screen clear color
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// Shader stuff
	shader = LS("shader");
	glUseProgram(shader);
	//

	// Generating vertex array
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	//

	// Generating buffers
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	//
	
	// Setting Vertex Attributes / Uniforms
	vtxID = glGetAttribLocation(shader, "vertex");
	glVertexAttribPointer(vtxID, 3, GL_FLOAT, GL_FALSE, sizeof(Vtx), (void*)0);
	glEnableVertexAttribArray(vtxID);

	nrmID = glGetAttribLocation(shader, "normal");
	glVertexAttribPointer(nrmID, 3, GL_FLOAT, GL_FALSE, sizeof(Vtx), (void*)12);
	glEnableVertexAttribArray(nrmID);

	clrID = glGetAttribLocation(shader, "color");
	glVertexAttribPointer(clrID, 4, GL_FLOAT, GL_FALSE, sizeof(Vtx), (void*)24);
	glEnableVertexAttribArray(clrID);

	do_frame = glGetUniformLocation(shader, "do_frame");
	
	mvpID = glGetUniformLocation(shader, "MVP");
	//

	// Testing shapes
	Field field = Field(
		glm::mat2x3(
			0, 0, 0,
			10, 0, 10
		), glm::vec4(1, 1, 1, 1)
	);

	Rpsm cube1 = Rpsm(
		glm::mat2x3(
			2, 0, 0,
			1, 1, 1
		), glm::vec4(1, 1, 1, 1)
	);

	Rpsm cube2 = Rpsm(
		glm::mat2x3(
			2, 2, 0,
			1, 1, 1
		), glm::vec4(1, 1, 1, 1)
	);

	Plane plane1 = Plane(
		glm::mat4x3(
			0, 0, 0,
			0, 1, 0,
			1, 1, 0,
			1, 0, 0
		), glm::mat4(
			1, 1, 1, 1,
			1, 1, 1, 1,
			1, 1, 1, 1,
			1, 1, 1, 1)
	);

	Plane plane2 = Plane(
		glm::mat4x3(
			0, 1, 0,
			0, 2, 0,
			1, 2, 0,
			1, 1, 0
		), glm::mat4(
			1, 1, 1, 1,
			1, 1, 1, 1,
			1, 1, 1, 1,
			1, 1, 1, 1)
	);

	cube1.v_inv();
	plane1.v_inv();
	//

	do {
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Processing Camera
		cam.update();
		glUniformMatrix4fv(mvpID, 1, GL_FALSE, &cam.MVP[0][0]);

		// Pumping vertices
		pump();

		// End of loop stuff
		glfwSwapBuffers(window);
		glfwPollEvents();
		do_inputs();
		//

		// Closing the window if the window should close
	} while (glfwWindowShouldClose(window) == 0);

	// Deallocating memory
	glDeleteProgram(shader);
	glfwDestroyWindow(window);
	glfwTerminate();
	//

	return;
}

void App::pump() {
	// Updating vertices
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vtx) * VERTICES.size(), &VERTICES.data()[0], GL_STATIC_DRAW);
	
	// Drawing the frame
	glUniform1i(do_frame, true);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLint) * FRAME.size(), &FRAME.data()[0], GL_STATIC_DRAW);
	glDrawElements(GL_LINES, FRAME.size(), GL_UNSIGNED_INT, 0);

	// Drawing tris
	glUniform1i(do_frame, false);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLint) * INDICES.size(), &INDICES.data()[0], GL_STATIC_DRAW);
	glDrawElements(GL_TRIANGLES, INDICES.size(), GL_UNSIGNED_INT, 0);
}

// WASD Movement, SPACE is up, LCONTROL = down
void App::do_inputs() {
	App* app = (App*)glfwGetWindowUserPointer(window);

	if (glfwGetKey(window, GLFW_KEY_W)) cam.trans({ 0, 0,  0.1 });
	if (glfwGetKey(window, GLFW_KEY_A)) cam.trans({  0.1, 0, 0 });
	if (glfwGetKey(window, GLFW_KEY_S)) cam.trans({ 0, 0, -0.1 });
	if (glfwGetKey(window, GLFW_KEY_D)) cam.trans({ -0.1, 0, 0 });
	if (glfwGetKey(window, GLFW_KEY_SPACE)) cam.trans({ 0,  0.1, 0 });
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL)) cam.trans({ 0, -0.1, 0 });
}
void App::CursorCallback(GLFWwindow* window, double xpos, double ypos) {
	App* app = (App*)glfwGetWindowUserPointer(window);

	float s = 1 / app->sensitivity;

	if (ypos*s >  89) glfwSetCursorPos(window, xpos,  89/s);
	if (ypos*s < -89) glfwSetCursorPos(window, xpos, -89/s);

	app->cam.rot_x(glm::radians(-ypos*s));
	app->cam.rot_y(glm::radians(xpos*s));
}
void App::KeyPrsCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	App* app = (App*)glfwGetWindowUserPointer(window);

	switch (key) {
	case GLFW_KEY_ESCAPE:
		glfwSetWindowShouldClose(window, true);
		break;
	}
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
