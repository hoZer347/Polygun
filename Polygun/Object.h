#pragma once

#include "Geometry.h"

class Object {
public:
	virtual void update()=0;

	bool cull = false;
	glm::vec3 pos, vec;

	Rpsm hit_box;

	int age = 0;
	int max_age = 0;
};

class Prj : public Object {
public:
	Prj(glm::vec3, glm::vec3, int=100);
	void update();

private:
};

class Enemy : public Object {
public:
	Enemy(glm::vec3);
	void update();
};
