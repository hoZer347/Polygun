#include "Camera.h"

void Camera::update() {
	Proj = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 1000.0f);

	look_from = camPos + trns + height;
	look_at = yaw * pitch * roll * (trgPos - camPos) + camPos + trns;

	View = glm::lookAt(
		look_from,
		look_at,
		up);
	Mode = glm::mat4(1.0f);

	MVP = Proj * View * Mode;
}

void Camera::rot_x(float ang) {
	roll = {
		1,			0,			0,
		0,			cos(ang),	-sin(ang),
		0,			sin(ang),	cos(ang)
	};
}

void Camera::rot_y(float ang) {
	pitch = {
		cos(ang),	0,			sin(ang),
		0,			1,			0,
		-sin(ang),	0,			cos(ang)
	};
}

void Camera::rot_z(float ang) {
	yaw = {
		cos(ang),	-sin(ang),	0,
		sin(ang),	cos(ang),	0,
		0,			0,			1
	};
}

void Camera::trans(glm::vec3 v) {
	trns += pitch*v;
}

void Camera::get_pos(glm::vec3& v1, glm::vec3& v2) {
	v1 = look_from;
	v2 = look_at;
}
