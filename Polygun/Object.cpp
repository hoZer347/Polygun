#include "Object.h"

Object::Object() {

}

Object::~Object() {

}

void Object::render(GLFWwindow* window) {
	for (auto& g : geometry)
		g->render(window);
}
