#pragma once

#include "Geometry.h"

class Plane : public Geometry {
public:
	Plane(const char* name) : Geometry(name) {
		load();
	};

	void load();
	void render();
};

