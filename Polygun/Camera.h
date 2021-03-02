#pragma once

#include <glm.hpp>
#include <glew.h>
#include <glut.h>
#include <glfw3.h>

#include <math.h>

class Camera {
public:
	void operator+=(glm::vec3);
	void operator-=(glm::vec3);

	void rotate(glm::vec3);
	void update();

	int age = 0;
	int distance = 5;
	glm::vec3 facing = glm::vec3(1, 0, 0);
	glm::vec3 rotation = glm::vec3(0, 0, 0);
	glm::vec3 pos = glm::vec3(0, 1, -5);
};

