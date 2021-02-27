#pragma once

#define GLFW_INCLUDE_NONE
#include <glm.hpp>
#include <glew.h>
#include <glut.h>
#include <glfw3.h>

#include <iostream>

#include <stdlib.h>
#include <stdio.h>

#include <vector>

class App {
public:
	App();
	~App();

	void init();


private:
	GLFWwindow* window;
	
	std::vector<glm::vec3> vertices = {
		{0.5f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.5f},
		{0.0f, 0.0f, 0.0f}
	};
};

