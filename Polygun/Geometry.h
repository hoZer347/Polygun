#pragma once

#include <glm.hpp>
#include <glew.h>
#include <glut.h>
#include <glfw3.h>

#include <vector>

class Geometry {
public:
	Geometry(const char*);
	~Geometry();

	void operator+=(glm::vec3);
	void operator-=(glm::vec3);
	void operator*=(int);
	void operator/=(int);

	virtual void load()=0;
	virtual void render(GLFWwindow*)=0;

	const char* name = NULL;
	int scale = 1;

	std::vector<GLfloat> vertices;

	glm::vec3 pos = { 0, 0, 0 };
};

