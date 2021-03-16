#pragma once

#include "Geometry.h"

class Object {
public:

	int age = 0;
};

class Prj : public Object {
public:
	Prj(glm::vec3);
};
