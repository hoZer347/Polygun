#include "Field.h"

#include <iostream>

Field::Field(int x, int y) {
	gen = [](glm::vec3& v) {
		float newX = v.x;
		float newY = (sin(v.x) + cos(v.z)) / 4;
		float newZ = v.z;

		if (v.y >= newY)
			return;

		v = glm::vec3(newX, newY, newZ);
	};

	x_size = x;
	y_size = y;

	Points* p = new Points();

	for (int i = -x_size; i < x_size; i++)
		for (int j = -y_size; j < y_size; j++) {
			glm::vec3 v1 = glm::vec3(i, 0, j);
			glm::vec3 v2 = glm::vec3(i, 0, j+1);
			glm::vec3 v3 = glm::vec3(i+1, 0, j+1);
			glm::vec3 v4 = glm::vec3(i+1, 0, j);

			gen(v1);
			gen(v2);
			gen(v3);
			gen(v4);

			p->v(v1);
			p->v(v2);
			p->v(v3);
			p->v(v1);
			p->v(v4);
			p->v(v3);

			p->f(v1);
			p->f(v2);
			p->f(v2);
			p->f(v3);
			p->f(v3);
			p->f(v4);
			p->f(v4);
			p->f(v1);
		}

	geo.push_back(p);
}

Field::~Field() {

}

void Field::load(std::function<void(glm::vec3&)> v) {
	gen = v;
}

void Field::get(glm::vec3& v) {
	gen(v);
}
