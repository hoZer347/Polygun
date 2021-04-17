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

	cdsID = glGetAttribLocation(shader, "texCoords");
	glVertexAttribPointer(cdsID, 2, GL_FLOAT, GL_FALSE, sizeof(Vtx), (void*)40);
	glEnableVertexAttribArray(cdsID);

	do_frame = glGetUniformLocation(shader, "do_frame");
	do_tex = glGetUniformLocation(shader, "do_tex");

	mvpID = glGetUniformLocation(shader, "MVP");
	mdlID = glGetUniformLocation(shader, "mode");
	viwID = glGetUniformLocation(shader, "view");
	prjID = glGetUniformLocation(shader, "proj");
	//

	// Setting Textures
	texID = glGetUniformLocation(shader, "tex");

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	int width, height;
	unsigned char* image = SOIL_load_image("cpt.png", &width, &height, 0, SOIL_LOAD_AUTO);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);

	glBindTexture(GL_TEXTURE_2D, 0);
	//

	// Populating area
	Field field = Field(
		glm::mat2x3(
			0, 0, 0,
			40, 0, 40
		), glm::vec4(1, 1, 1, 1),
		1, tst
	);

	field.add_face();

	obj = ObjManager(&cam, &field);

	Enemy* e1 = new Enemy(glm::vec3(10, 0, 10));
	Enemy* e2 = new Enemy(glm::vec3(15, 0, 15));
	Enemy* e3 = new Enemy(glm::vec3(10, 0, 15));
	 
	obj.add_enemy(e1);
	obj.add_enemy(e2);
	obj.add_enemy(e3);

	Sphere sphere = Sphere(
		glm::vec3(0, 10, 0),
		glm::vec2(20, 20),
		glm::vec4(1, 1, 1, 1),
		10
	);

	T = INDICES.size();

	plane = Plane(
		glm::mat4x3(
			0, 10, 10,
			0, 10, 20,
			0, 20, 20,
			0, 20, 10
		), glm::vec4(1, 1, 1, 0.1f)
	);

	VERTICES[plane.v1_index + 0].tex = { 1, 0 };
	VERTICES[plane.v1_index + 1].tex = { 1, 1 };
	VERTICES[plane.v1_index + 2].tex = { 0, 1 };
	VERTICES[plane.v1_index + 3].tex = { 0, 0 };

	VERTICES[plane.v1_index + 0].nrm = { 1, 0, 0 };
	VERTICES[plane.v1_index + 1].nrm = { 1, 0, 0 };
	VERTICES[plane.v1_index + 2].nrm = { 1, 0, 0 };
	VERTICES[plane.v1_index + 3].nrm = { 1, 0, 0 };
	//

	int age = 0;

	do {
		double start_time = glfwGetTime();

		if (dyn_light) {
			GLuint lp = glGetUniformLocation(shader, "lightPos");
			glm::vec3 p, p1;
			cam.get_pos(p, p1);
			glUniform3f(lp, p.x, p.y, p.z);
		} else field.get(cam.trns);

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Processing Camera
		cam.update();
		glUniformMatrix4fv(mvpID, 1, GL_FALSE, &cam.MVP[0][0]);
		glUniformMatrix4fv(mdlID, 1, GL_FALSE, &cam.Mode[0][0]);
		glUniformMatrix4fv(viwID, 1, GL_FALSE, &cam.View[0][0]);
		glUniformMatrix4fv(prjID, 1, GL_FALSE, &cam.Proj[0][0]);
		//

		// Processing objects
		obj.update();
		 
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
	glUniform1i(do_frame, false);

	// Drawing tris
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLint) * INDICES.size(), &INDICES.data()[0], GL_STATIC_DRAW);
	//glDrawElements(GL_TRIANGLES, INDICES.size(), GL_UNSIGNED_INT, 0);

	// Drawing Textures
	glUniform1i(do_tex, true);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLint) * 6, &INDICES.data()[T], GL_STATIC_DRAW);
	glBindTexture(GL_TEXTURE_2D, texture);
	glDrawElements(GL_TRIANGLES, INDICES.size(), GL_UNSIGNED_INT, 0);
	glUniform1i(do_tex, false);
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

	GLfloat s = 1 / app->sensitivity;

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

	switch (button) {
	case GLFW_MOUSE_BUTTON_LEFT:
		app->obj.add_proj();
		break;
	case GLFW_MOUSE_BUTTON_RIGHT:
		if (app->dyn_light)
			app->dyn_light = false;
		else {
			app->dyn_light = true;
			//GLuint lp = glGetUniformLocation(app->shader, "lightPos");
			//glUniform3f(lp, 100, 100, 100);
		}
		break;
	}
}
void App::ResizeCallback(GLFWwindow* window, int width, int height) {
	App* app = (App*)glfwGetWindowUserPointer(window);
}
void App::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	App* app = (App*)glfwGetWindowUserPointer(window);
}
