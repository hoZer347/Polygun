#include "Camera.h"

void Camera::rotate(glm::vec3 v) {
	rotation.x = v.x;
	rotation.y = v.y;
	rotation.z = v.z;
}

void Camera::update() {
	glTranslatef(pos.x, pos.y, pos.z);
	// glRotatef(rotation.x, 1, 0, 0);
	// glRotatef(rotation.z, 0, 0, 1);
}

void Camera::operator+=(glm::vec3 v) {
	pos.x += v.x;
	pos.y += v.y;
	pos.z += v.z;
}
void Camera::operator-=(glm::vec3 v) {
	pos.x -= v.x;
	pos.y -= v.y;
	pos.z -= v.z;
}
