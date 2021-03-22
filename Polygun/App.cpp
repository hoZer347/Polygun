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
	shader = LS("gouraud");
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
	projID = glGetUniformLocation(shader, "projection_mat");
	viewID = glGetUniformLocation(shader, "view_mat");
	wrldID = glGetUniformLocation(shader, "world_mat");
	shineID = glGetUniformLocation(shader, "shine");
	//

	// Populating area
	Sphere sphere1 = Sphere(
		glm::vec3(100, 0, 100),	// Position
		glm::vec2(20, 20),		// Longs / Lats
		glm::vec4(1, 0, 0, 1),	// Color
		10
	);
	Sphere sphere2 = Sphere(
		glm::vec3(140, 0, 100),
		glm::vec2(20, 20),
		glm::vec4(0, 1, 0, 1),
		10
	);
	player.cam.trans({ 100, 0, 0 });
	//

	int age = 0;

	do {
		double start_time = glfwGetTime();
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Processing Camera
		player.update();
		//field.get(player.cam.trns);
		glUniformMatrix4fv(mvpID, 1, GL_FALSE, &player.cam.MVP[0][0]);
		glUniformMatrix4fv(projID, 1, GL_FALSE, &player.cam.Proj[0][0]);
		glUniformMatrix4fv(viewID, 1, GL_FALSE, &player.cam.View[0][0]);
		glUniformMatrix4fv(wrldID, 1, GL_FALSE, &player.cam.Mode[0][0]);
		//

		// Processing objects
		//obj.update();
		 
		// Pumping vertices
		pump();
		// End of loop stuff
		glfwSwapBuffers(window);
		glfwPollEvents();
		do_inputs();
		//

		double end_time = glfwGetTime();
		
		// std::cout << end_time-start_time << std::endl;

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

	GLuint phong = glGetUniformLocation(shader, "phong");

	// Drawing first sphere
	glUniform1i(do_frame, false);
	glUniform1i(phong, false);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLint) * INDICES.size()/2, &INDICES.data()[0], GL_STATIC_DRAW);
	glDrawElements(GL_TRIANGLES, INDICES.size()/2, GL_UNSIGNED_INT, 0);

	// Drawing second sphere
	glUniform1i(do_frame, false);
	glUniform1i(phong, true);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLint) * INDICES.size()/2, &INDICES.data()[INDICES.size()/2], GL_STATIC_DRAW);
	glDrawElements(GL_TRIANGLES, INDICES.size()/2, GL_UNSIGNED_INT, 0);
}

// WASD Movement, SPACE is up, LCONTROL = down
void App::do_inputs() {
	App* app = (App*)glfwGetWindowUserPointer(window);

	if (glfwGetKey(window, GLFW_KEY_W)) player.cam.trans({ 0, 0,  1 });
	if (glfwGetKey(window, GLFW_KEY_A)) player.cam.trans({  1, 0, 0 });
	if (glfwGetKey(window, GLFW_KEY_S)) player.cam.trans({ 0, 0, -1 });
	if (glfwGetKey(window, GLFW_KEY_D)) player.cam.trans({ -1, 0, 0 });
	if (glfwGetKey(window, GLFW_KEY_SPACE)) player.cam.trans({ 0,  1, 0 });
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL)) player.cam.trans({ 0, -1, 0 });
}
void App::CursorCallback(GLFWwindow* window, double xpos, double ypos) {
	App* app = (App*)glfwGetWindowUserPointer(window);

	GLfloat s = 1 / app->sensitivity;

	if (ypos*s >  89) glfwSetCursorPos(window, xpos,  89/s);
	if (ypos*s < -89) glfwSetCursorPos(window, xpos, -89/s);

	app->player.cam.rot_x(glm::radians(-ypos*s));
	app->player.cam.rot_y(glm::radians(xpos*s));
}
void App::KeyPrsCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	App* app = (App*)glfwGetWindowUserPointer(window);

	switch (key) {
	case GLFW_KEY_ESCAPE:
		glfwSetWindowShouldClose(window, true);
		break;
	case GLFW_KEY_EQUAL:
		if (action == GLFW_PRESS) {
			app->shine += 2.0f;
			if (app->shine < 1) app->shine = 1;
			glUniform1f(app->shineID, app->shine);
		}
		break;
	case GLFW_KEY_MINUS:
		if (action == GLFW_PRESS) {
			app->shine -= 2.0f;
			if (app->shine < 1) app->shine = 1;
			glUniform1f(app->shineID, app->shine);
		}
		break;
	case GLFW_KEY_M:
		if (action == GLFW_PRESS) {
			GLuint toggle = glGetUniformLocation(app->shader, "do_A");
			app->do_A = app->do_A * -1 + 1;
			glUniform1i(toggle, app->do_A);
		}
		break;
	case GLFW_KEY_N:
		if (action == GLFW_PRESS) {
			GLuint toggle = glGetUniformLocation(app->shader, "do_D");
			app->do_D = app->do_D * -1 + 1;
			glUniform1i(toggle, app->do_D);
		}
		break;
	case GLFW_KEY_B:
		if (action == GLFW_PRESS) {
			GLuint toggle = glGetUniformLocation(app->shader, "do_S");
			app->do_S = app->do_S * -1 + 1;
			glUniform1i(toggle, app->do_S);
		}
		break;
	case GLFW_KEY_I:
		if (action == GLFW_PRESS) {
			GLuint toggle = glGetUniformLocation(app->shader, "x_scale");
			app->x_scale += 0.5;
			glUniform1d(toggle, app->x_scale);
		}
		break;
	case GLFW_KEY_J:
		if (action == GLFW_PRESS) {
			GLuint toggle = glGetUniformLocation(app->shader, "x_scale");
			if (app->x_scale < 1.1) break;
			app->x_scale -= 0.5;
			glUniform1d(toggle, app->x_scale);
		}
		break;
	case GLFW_KEY_K:
		if (action == GLFW_PRESS) {
			GLuint toggle = glGetUniformLocation(app->shader, "y_scale");
			app->y_scale += 0.5;
			glUniform1d(toggle, app->y_scale);
		}
		break;
	case GLFW_KEY_L:
		if (action == GLFW_PRESS) {
			GLuint toggle = glGetUniformLocation(app->shader, "y_scale");
			if (app->y_scale < 1.1) break;
			app->y_scale -= 0.5;
			glUniform1d(toggle, app->y_scale);
		}
		break;
	case GLFW_KEY_R:
		if (action == GLFW_PRESS) {
			GLuint toggle1 = glGetUniformLocation(app->shader, "x_scale");
			GLuint toggle2 = glGetUniformLocation(app->shader, "y_scale");
			app->x_scale = 1;
			app->y_scale = 1;
			glUniform1d(toggle1, app->x_scale);
			glUniform1d(toggle2, app->y_scale);
		}
		break;
	case GLFW_KEY_H:
		if (action == GLFW_PRESS) {
			GLuint toggle = glGetUniformLocation(app->shader, "blinn");
			app->Hint = app->Hint*-1+1;
			glUniform1i(toggle, app->Hint);
		}
		break;
	}
}
void App::MouseBCallback(GLFWwindow* window, int button, int action, int mods) {
	App* app = (App*)glfwGetWindowUserPointer(window);

	//switch (button) {
	//case GLFW_MOUSE_BUTTON_LEFT:
	//	app->obj.add_proj();
	//	break;
	//}
}
void App::ResizeCallback(GLFWwindow* window, int width, int height) {
	App* app = (App*)glfwGetWindowUserPointer(window);
}
void App::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	App* app = (App*)glfwGetWindowUserPointer(window);
}
