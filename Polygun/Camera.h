#pragma once

#include <glm.hpp>
#include <glew.h>
#include <glut.h>
#include <glfw3.h>
#include <gtc/matrix_transform.hpp>

#include <iostream>

class Camera {
public:
	Camera();

	void operator+=(glm::vec3);
	void operator-=(glm::vec3);

	void rotate(glm::vec3);
	void update();

	int distance = 5;

	glm::vec3 up =     {  0,  1,  0 }; // Up Vector
	glm::vec3 right =  {  0,  0,  1 }; // Right Vector
	glm::vec3 trgPos = {  0,  0,  0 }; // Position looked at by camera
	glm::vec3 camPos = {  0,  1, -5 }; // Camera's position
	glm::vec3 rotation = { 0, 0, 0 };
	glm::mat4x4 r_mat;
	glm::mat4x4 view;				   // Translation Matrix

	glm::mat3x3 roll;
	glm::mat3x3 yaw;
	glm::mat3x3 pitch;
};

