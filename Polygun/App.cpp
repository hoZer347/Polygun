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

		// --Test objects-- //
	Object* o = new Object();
	Cube* cube = new Cube();
	*cube += glm::vec3(-0.5, 0.01, -0.5);
	o->geo.push_back(cube);
	objects.push_back(o);

	field = new Field(50, 50);
	objects.push_back(field);
		// ---------------- //
}

App::~App() {

}

void App::init() {
	// Setting screen clear color
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// Shader stuff
	shader = LS("shader");
	mvpID = glGetUniformLocation(shader, "MVP");
	clrID = glGetUniformLocation(shader, "fcolor");
	normalID = glGetUniformLocation(shader, "normal");
	viewID = glGetUniformLocation(shader, "view_mat");
	glUseProgram(shader);
	//

	// Generating vertex array
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
	//

	// Generating vertex buffer
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	//

	do {
		// Starting fps timer
		double start_time = glfwGetTime();

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Pumping vertices into vertex buffer
		for (auto& o : objects)
			for (auto& g : o->geo) {
				if (do_verts) vertices.insert(vertices.end(), g->vertices.begin(), g->vertices.end());
				if (do_frame) frame.insert(frame.end(), g->frame.begin(), g->frame.end());
			}

		std::cout << "Vertex count: ~" << vertices.size() + frame.size() << std::endl;

		pump(vertices, GL_TRIANGLES);
		pump(frame, GL_LINE_LOOP);
		//

		// Assigning camera position
		field->get(cam.pos);
		cam.Projection = glm::perspective(cam.FoV, cam.aspectRatio, 0.1f, cam.renderDistance);
		//

		// Showing changes
		glfwSwapBuffers(window);

		// Doing inputs
		glfwPollEvents();
		do_inputs();
		//

		// Getting fps
		double end_time = glfwGetTime();
		double elapsed_time = end_time - start_time;

		std::cout << "Time elapsed: " << (int)(1000*elapsed_time) << "ms, FPS: " << (int)(1/elapsed_time) << std::endl;

		while (end_time - start_time < 1 / 60)
			end_time = glfwGetTime();
		//

		// Testing if the window should close
	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

	// Deallocating memory
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteProgram(shader);
	glDeleteVertexArrays(1, &VertexArrayID);
	glfwTerminate();
	//

	return;
}

void App::pump(std::vector<GLfloat>& v, GLenum type) {
	int size = v.size();
	
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * size, v.data(), GL_DYNAMIC_DRAW);

	glUniformMatrix4fv(mvpID, 1, GL_FALSE, &cam.update()[0][0]);


	//Normals for lighting
	glm::mat4 transformationMatrix = glm::mat4(1);
	glm::vec3 normal = glm::vec3(0, -1, 0); //TODO calc this
	glm::vec4 vtxNormal = glm::transpose(glm::inverse(cam.View * transformationMatrix)) * glm::vec4(normal, 1);
	glUniform4f(normalID, vtxNormal[0], vtxNormal[1], vtxNormal[2], vtxNormal[3]);
	glUniformMatrix4fv(viewID, 1, GL_FALSE, glm::value_ptr(cam.View));

	glEnableVertexAttribArray(0);
	//glEnableVertexAttribArray(1);
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

// WASD Movement, SPACE is up, LCONTROL = down
void App::do_inputs() {
	// Calculating normals so that moving diagonally isn't faster
	float dx = sqrt(abs(pow((glfwGetKey(window, GLFW_KEY_A)), 2) - pow((glfwGetKey(window, GLFW_KEY_D)), 2)))/10;
	float dy = sqrt(abs(pow((glfwGetKey(window, GLFW_KEY_W)), 2) - pow((glfwGetKey(window, GLFW_KEY_S)), 2)))/10;

	// Moving along normals
	if (glfwGetKey(window, GLFW_KEY_W)) cam += glm::vec3(0, 0, -dy);
	if (glfwGetKey(window, GLFW_KEY_A)) cam += glm::vec3(-dx, 0, 0);
	if (glfwGetKey(window, GLFW_KEY_S)) cam += glm::vec3(0, 0, dy);
	if (glfwGetKey(window, GLFW_KEY_D)) cam += glm::vec3(dx, 0, 0);

	// Moving up or down
	if (glfwGetKey(window, GLFW_KEY_SPACE)) cam += glm::vec3(0, 0.1, 0);
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL)) cam += glm::vec3(0, -0.1, 0);
}

void App::CursorCallback(GLFWwindow* window, double xpos, double ypos) {
	App* app = (App*)glfwGetWindowUserPointer(window);

	if (ypos > 89 * 4) ypos = 89 * 4;
	if (ypos < -89 * 4) ypos = -89 * 4;

	app->cam.rotate(glm::vec3(0, xpos/4, ypos/4));

	glfwSetCursorPos(window, xpos, ypos);
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
