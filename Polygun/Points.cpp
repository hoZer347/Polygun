#include "Points.h"

Points::Points() {

}

Points::~Points() {

}

void Points::v(glm::vec3 v) {
	vertices.push_back(v.x);
	vertices.push_back(v.y);
	vertices.push_back(v.z);
}

void Points::f(glm::vec3 v) {
	frame.push_back(v.x);
	frame.push_back(v.y);
	frame.push_back(v.z);
}
