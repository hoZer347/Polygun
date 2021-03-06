#pragma once

#include <glm.hpp>
#include <glfw3.h>
#include <gtc/matrix_transform.hpp>
#include <gtx/transform.hpp>
#include <gtx/rotate_vector.hpp>

// Manages how the player views the scene
class Camera {
public:
	Camera();
	~Camera();

	void operator+=(glm::vec3);

	glm::mat4& update();
	void rotate(glm::vec3);
	void translate(glm::vec3);

	glm::mat4 Projection, View, Model, MVP;
	glm::mat3 roll, yaw, pitch;
	glm::vec3 trans = glm::vec3(0, 0, 0);
	glm::vec3 camPos = glm::vec3(0, 0, 0), trgPos = glm::vec3(0, 0, 0), up = glm::vec3(0, 1, 0);
	glm::vec3 pos = { 0, 0, 0 };
	glm::vec3 height = { 0, 1.5, 0 };
	float FoV = glm::radians(45.0f);
	float aspectRatio = 4.0f/3.0f;
	float renderDistance = 100.0f;
};

