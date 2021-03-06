#pragma once

#include "Object.h"
#include "Points.h"

#include <functional>

// Generates a field in which objects can interact
class Field : public Object {
public:
	Field(int, int);
	~Field();

	void load(std::function<void(glm::vec3&)>);
	void get(glm::vec3&);
private:

	float x_size, y_size;

	std::function<void(glm::vec3&)> gen;
};

