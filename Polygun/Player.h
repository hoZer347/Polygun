#pragma once

#include "glm.hpp"

#include "Object.h"
#include "Camera.h"

class Player : public Object {
public:
	Player() {};
	~Player() {};

	Camera cam;
};

