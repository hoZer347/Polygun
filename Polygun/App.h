#pragma once

#include <glm.hpp>
#include <glew.h>
#include <glut.h>
#include <glfw3.h>

#include <iostream>

#include <stdlib.h>
#include <stdio.h>

#include <vector>

#include "Object.h"
#include "Player.h"

class App {
public:
	App();
	~App();

	void init();
	void move_player() {
		for (auto& i : player.vertices)
			i += 1;
	};

	// Called every time a input is detected
	static void KeyboardCallBack(GLFWwindow*, int, int, int, int);

private:
	GLFWwindow* window;

	Player player;

	std::vector<Object*> objects;
};

