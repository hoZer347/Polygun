#include "Geometry.h"

void Geometry::operator+=(glm::vec3 v) {
	auto it = vertices.begin();

	while (it != vertices.end()) {
		*it += v.x;
		it++;
		*it += v.y;
		it++;
		*it += v.z;
		it++;
	}

	it = frame.begin();

	while (it != frame.end()) {
		*it += v.x;
		it++;
		*it += v.y;
		it++;
		*it += v.z;
		it++;
	}
}

void Geometry::operator*=(GLfloat f) {
	for (auto& v : vertices)
		v *= f;

	for (auto& v : frame)
		v *= f;
}
