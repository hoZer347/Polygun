#include "Geometry.h"

#include <iostream>

std::vector<GLfloat> VERTICES;
std::vector<GLfloat> FRAME;
std::vector<GLfloat> COLOR;

void Vtx::load(glm::vec3 pos, glm::vec4 color) {
	VERTICES.push_back(pos.x);
	x = VERTICES[VERTICES.size()-1];
	VERTICES.push_back(pos.y);
	y = VERTICES[VERTICES.size()-1];
	VERTICES.push_back(pos.z);
	z = VERTICES[VERTICES.size()-1];

	COLOR.push_back(color.r);
	r = COLOR[COLOR.size()-1];
	COLOR.push_back(color.g);
	g = COLOR[COLOR.size()-1];
	COLOR.push_back(color.b);
	b = COLOR[COLOR.size()-1];
	COLOR.push_back(color.a);
	a = COLOR[COLOR.size()-1];
};

void Line::load(glm::mat3x2 pos, glm::vec4 color) {
	FRAME.push_back(pos[0][0]);
	x1 = FRAME[FRAME.size()-1];
	FRAME.push_back(pos[1][0]);
	y1 = FRAME[FRAME.size()-1];
	FRAME.push_back(pos[2][0]);
	z1 = FRAME[FRAME.size()-1];
	FRAME.push_back(pos[0][1]);
	x2 = FRAME[FRAME.size()-1];
	FRAME.push_back(pos[1][1]);
	y2 = FRAME[FRAME.size()-1];
	FRAME.push_back(pos[2][1]);
	z2 = FRAME[FRAME.size()-1];
};

void Tri::load(glm::mat3 v, glm::mat4x3 c) {
	v1.load(
		glm::vec3(v[0][0], v[1][0], v[2][0]),
		glm::vec4(c[0][0], c[1][0], c[2][0], c[3][0]));
	v2.load(
		glm::vec3(v[0][1], v[1][1], v[2][1]),
		glm::vec4(c[0][1], c[1][1], c[2][1], c[3][1]));
	v3.load(
		glm::vec3(v[0][2], v[1][2], v[2][2]),
		glm::vec4(c[0][2], c[1][2], c[2][2], c[3][2]));
}
