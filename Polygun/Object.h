#pragma once

#include <glm.hpp>
#include <glew.h>
#include <glut.h>
#include <glfw3.h>

#include <vector>

#include "Geometry.h"

class Object {
public:
	Object();
	~Object();

	void render(GLFWwindow*);

	std::vector<Geometry*> geometry;
};
