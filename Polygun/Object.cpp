#include "Object.h"

Object::Object() {
	for (auto& i : vertices)
		i / 20;
}

Object::~Object() {

}


