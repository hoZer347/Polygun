#include "Camera.h"

Camera::Camera() {

}

Camera::~Camera() {

}

void Camera::operator+=(glm::vec3 v) {
	v = yaw * v;
	camPos += v;
	trgPos += v;
}

glm::mat4& Camera::update() {
	View = glm::lookAt(
		camPos,
		yaw*(trgPos-camPos)+camPos,
		up
	);
	Model = glm::mat4(1.0f);
	MVP = Projection * View * Model;
	return MVP;
}

void Camera::rotate(glm::vec3 v) {
	v = glm::radians(v);

	roll = glm::mat3(
		cos(v.x),	-sin(v.x),	0,
		sin(v.x),	cos(v.x),	0,
		0,			0,			1
	);

	yaw = glm::mat3(
		cos(v.y),	0,			sin(v.y),
		0,			1,			0,
		-sin(v.y),	0,			cos(v.y)
	);

	pitch = glm::mat3(
		1,			0,			0,
		0,			cos(v.z),	-sin(v.z),
		0,			sin(v.z),	cos(v.z)
	);

	rot = v;
}

void Camera::translate(glm::vec3 v) {
	trans = v;
}
