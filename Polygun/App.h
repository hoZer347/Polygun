#pragma once

#include <glm.hpp>
#include <glew.h>
#include <glut.h>
#include <glfw3.h>

#include <iostream>

#include <stdlib.h>
#include <stdio.h>

#include <vector>

#include "Object.h"
#include "Player.h"
#include "Camera.h"
#include "Plane.h"

#include "math.h"

class App {
public:
	App();
	~App();

	void mouse() { glfwGetCursorPos(window, &mx, &my); }
	void init();

	// Called every time a input is detected
	static void KeyboardCallBack(GLFWwindow*, int, int, int, int);

private:
	GLFWwindow* window;

	Player player;
	Camera cam;

	double mx = 0, my = 0;

	std::vector<Object*> objects;
};

