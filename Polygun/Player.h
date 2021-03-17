#pragma once

#include "glm.hpp"

#include "Object.h"
#include "Camera.h"

class Player {
public:
	Player() {};
	~Player() {};

	void update();

	Camera cam;
};

