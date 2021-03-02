#include "Object.h"

Object::Object() {

}

Object::~Object() {

}

void Object::render() {
	for (auto& g : geometry)
		g->render();
}