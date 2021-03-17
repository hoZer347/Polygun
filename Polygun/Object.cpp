#include "Object.h"

#include <iostream>

Prj::Prj(glm::vec3 p, glm::vec3 v, int m) {
	hit_box = Rpsm(
		glm::mat2x3(
			p.x, p.y, p.z,
			0.1, 0.1, 0.1
		), glm::vec4(1, 1, 1, 1)
	);
	vec = v;
	max_age = m;
}
void Prj::update() {
	hit_box += vec;
	pos += vec;
	age++;
}

Enemy::Enemy(glm::vec3 v) {
	hit_box = Rpsm(
		glm::mat2x3(
			v.x, v.y, v.z,
			1, 1, 1
		), glm::vec4(1)
	);

	vec = { 0.01, 0, 0 };
	pos = v;
	max_age = 1;
}
void Enemy::update() {
	pos += glm::vec3(vec);
	hit_box.set_pos(pos - glm::vec3(0.5, 0, 0.5));
}
