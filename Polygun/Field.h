#pragma once

#include "Object.h"
#include "Points.h"

#include <functional>

// Generates a field in which objects can interact
// gen is the lambda that determines the manifestation of the field
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

