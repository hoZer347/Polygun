#pragma once

#include "Geometry.h"

class Points : public Geometry {
public:
	Points();
	~Points();
	
	void v(glm::vec3);
	void f(glm::vec3);
};

