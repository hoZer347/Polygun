#include "Camera.h"

Camera::Camera() {

}

void Camera::rotate(glm::vec3 v) {
	rotation = v;

	std::cout << rotation.y << ":" << rotation.z << std::endl;

	glm::vec3 r = glm::radians(v);

	roll = {
	cos(r.x),	-sin(r.x),	0,
	sin(r.x),	cos(r.x),	0,
	0,			0,			1
	};

	yaw = {
		cos(r.y),	0,			sin(r.y),
		0,			1,			0,
		-sin(r.y),	0,			cos(r.y)
	};

	pitch = {
		1,			0,			0,
		0,			cos(r.z),	-sin(r.z),
		0,			sin(r.z),	cos(r.z)
	};
}

void Camera::update() {
	glm::vec3 r = glm::radians(rotation);
	glRotatef(rotation.y, 0, 1, 0);
	glRotatef(rotation.z, cos(r.y), 0, sin(r.y));
	glTranslatef(camPos.x, camPos.y, camPos.z);
}

void Camera::operator+=(glm::vec3 v) {
	glm::vec3 r = glm::radians(rotation);

	v = yaw*v;

	camPos += v;
	trgPos += v;
}
void Camera::operator-=(glm::vec3 v) {
	camPos -= v;
	trgPos -= v;
}
