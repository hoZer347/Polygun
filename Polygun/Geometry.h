#pragma once

#include <glfw3.h>
#include <glm.hpp>

#include <vector>

class Geometry {
public:
	void operator+=(glm::vec3);
	void operator*=(GLfloat);

	std::vector<GLfloat> vertices;
	std::vector<GLfloat> frame;
private:
};

