#include "Camera.h"

Camera::Camera() {
	camPos = glm::vec3(0, 0, 3);
}

Camera::~Camera() {

}

// Used for camera movement
// Translates relative to the camera's yaw, as per fps controls
void Camera::operator+=(glm::vec3 v) {
	v = yaw * v;
	pos += v;
}

// Generates MVP matrix
glm::mat4& Camera::update() {
	View = glm::lookAt(
		camPos+pos+height,
		yaw*pitch*(trgPos-camPos)+camPos+pos+height,
		up
	);
	Model = glm::mat4(1.0f);
	MVP = Projection * View * Model;
	return MVP;
}

// Generates rotation matrices based on v: glm::vec3(roll, yaw, pitch)
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
}
