#pragma once

#include "Geometry.h"

class Cube : public Geometry {
public:
	Cube(const char* name) : Geometry(name) {
		load();
	};

	// Inheritted
	void load();
	void render(GLFWwindow*);
};

