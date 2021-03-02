#include "Geometry.h"

Geometry::Geometry(const char* n="shape") {
	name = n;
}

Geometry::~Geometry() {

}

void Geometry::operator+=(glm::vec3 V) {
	auto v = vertices.begin();

	while (v != vertices.end()) {
		*v += V.x;
		v++;
		*v += V.y;
		v++;
		*v += V.z;
		v++;
	}
}
void Geometry::operator-=(glm::vec3 V) {
	auto v = vertices.begin();

	while (v != vertices.end()) {
		*v -= V.x;
		v++;
		*v -= V.y;
		v++;
		*v -= V.z;
		v++;
	}
}
void Geometry::operator*=(int i) {
	for (auto& v : vertices)
		v *= i;

	scale *= i;
}
void Geometry::operator/=(int i) {
	for (auto& v : vertices)
		v /= i;

	scale /= i;
}
