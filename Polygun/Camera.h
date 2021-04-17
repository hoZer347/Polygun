#pragma once

#define GLM_FORCE_RADIANS

#include "glm.hpp"
#include <gtc/matrix_transform.hpp>

class Camera {
public:
	Camera();

	void update();

	void rot_x(float);
	void rot_y(float);
	void rot_z(float);

	void trans(glm::vec3);

	void get_pos(glm::vec3&, glm::vec3&);
	glm::vec3 get_pos1();

	glm::mat4 MVP;
	glm::vec3 trns = { -1, 0, -1 };
	glm::mat4 Proj, View, Mode;

private:
	glm::vec3 camPos = glm::vec3(0, 0, 0), trgPos = glm::vec3(0, -1, 5), up = glm::vec3(0, 1, 0);
	glm::vec3 look_at, look_from;
	glm::vec3 height = { 0, 1, 0 };
	glm::mat3 roll, yaw, pitch;
};

