#include "Geometry.h"

#include <iostream>

std::vector<Vtx> VERTICES;
std::vector<GLint> INDICES;
std::vector<GLint> FRAME;

void Geometry::operator+=(glm::vec3 v) {
	for (auto& i : vps)
		i->pos += v;
}
void Geometry::operator*=(GLfloat f) {
	for (auto& i : vps)
		i->pos *= f;
}

Tri::Tri(glm::mat3 pts, glm::mat3x4 clr) {
	glm::vec3 norm = { 0, 0, 0 };  

	v1_index = VERTICES.size();
	VERTICES.push_back({ pts[0], norm, clr[0] });
	VERTICES.push_back({ pts[1], norm, clr[1] });
	VERTICES.push_back({ pts[2], norm, clr[2] });
	v2_index = VERTICES.size()-1;

	int i0 = VERTICES.size()-3;
	int i1 = VERTICES.size()-2;
	int i2 = VERTICES.size()-1;

	mk_ind(i2, i1, i0);

	f1_index = FRAME.size();
	FRAME.push_back(i0);
	FRAME.push_back(i1);
	FRAME.push_back(i1);
	FRAME.push_back(i2);
	FRAME.push_back(i2);
	FRAME.push_back(i0);
	f2_index = FRAME.size()-1;
}
Tri::Tri(int i0, int i1, int i2) {
	mk_ind(i0, i1, i2);
	v1_index = std::min(i0, std::min(i1, i2));
	v2_index = std::max(i0, std::max(i1, i2));
	f1_index = std::min(i0, std::min(i1, i2));
	f2_index = std::max(i0, std::max(i1, i2));
}
void Tri::mk_ind(int i0, int i1, int i2) {
	vp1 = &VERTICES[i0];
	vp2 = &VERTICES[i1];
	vp3 = &VERTICES[i2];

	INDICES.push_back(i0);
	INDICES.push_back(i1);
	INDICES.push_back(i2);
}
void Tri::v_inv() {
	std::reverse(VERTICES.begin() + v1_index, VERTICES.begin() + v2_index);
}

Plane::Plane(glm::mat4x3 pts, glm::mat4 clr) {
	glm::vec3 norm = { 0, 0, 0 };

	v1_index = VERTICES.size();
	VERTICES.push_back({ pts[0], norm, clr[0] });
	VERTICES.push_back({ pts[1], norm, clr[1] });
	VERTICES.push_back({ pts[2], norm, clr[2] });
	VERTICES.push_back({ pts[3], norm, clr[3] });
	v2_index = VERTICES.size()-1;

	int i0 = VERTICES.size()-4;
	int i1 = VERTICES.size()-3;
	int i2 = VERTICES.size()-2;
	int i3 = VERTICES.size()-1;

	mk_ind(i3, i2, i1, i0);

	f1_index = FRAME.size();
	FRAME.push_back(i0);
	FRAME.push_back(i1);
	FRAME.push_back(i1);
	FRAME.push_back(i2);
	FRAME.push_back(i2);
	FRAME.push_back(i3);
	FRAME.push_back(i3);
	FRAME.push_back(i0);
	f2_index = FRAME.size()-1;
}
Plane::Plane(int i0, int i1, int i2, int i3) {
	mk_ind(i0, i1, i2, i3);
	v1_index = std::min(i0, std::min(i1, std::min(i2, i3)));
	v2_index = std::max(i0, std::max(i1, std::max(i2, i3)));
	f1_index = std::min(i0, std::min(i1, std::min(i2, i3)));
	f2_index = std::max(i0, std::max(i1, std::max(i2, i3)));
}
void Plane::mk_ind(int i0, int i1, int i2, int i3) {
	tri1 = Tri(i0, i1, i2);
	tri2 = Tri(i2, i3, i0);

	for (auto& i : tri1.vps) vps.push_back(i);
	for (auto& i : tri2.vps) vps.push_back(i);
}
void Plane::v_inv() {
	std::reverse(VERTICES.begin()+v1_index, VERTICES.begin()+v2_index);
}

Rpsm::Rpsm(glm::mat2x3 dims, glm::vec4 clr) {
	glm::vec3 norm = { 0, 0, 0 };

	GLfloat x1 = dims[0][0];
	GLfloat y1 = dims[0][1];
	GLfloat z1 = dims[0][2];
	GLfloat x2 = dims[1][0];
	GLfloat y2 = dims[1][1];
	GLfloat z2 = dims[1][2];

	glm::vec3 p0 = { x1     , y1     , z1      };
	glm::vec3 p1 = { x1 + x2, y1     , z1      };
	glm::vec3 p2 = { x1     , y1 + y2, z1      };
	glm::vec3 p3 = { x1 + x2, y1 + y2, z1      };
	glm::vec3 p4 = { x1     , y1     , z1 + z2 };
	glm::vec3 p5 = { x1 + x2, y1     , z1 + z2 };
	glm::vec3 p6 = { x1     , y1 + y2, z1 + z2 };
	glm::vec3 p7 = { x1 + x2, y1 + y2, z1 + z2 };

	v1_index = VERTICES.size();
	VERTICES.push_back({ p0, norm, clr });
	VERTICES.push_back({ p1, norm, clr });
	VERTICES.push_back({ p2, norm, clr });
	VERTICES.push_back({ p3, norm, clr });
	VERTICES.push_back({ p4, norm, clr });
	VERTICES.push_back({ p5, norm, clr });
	VERTICES.push_back({ p6, norm, clr });
	VERTICES.push_back({ p7, norm, clr });
	v2_index = VERTICES.size()-1;

	int i0 = VERTICES.size()-8;
	int i1 = VERTICES.size()-7;
	int i2 = VERTICES.size()-6;
	int i3 = VERTICES.size()-5;
	int i4 = VERTICES.size()-4;
	int i5 = VERTICES.size()-3;
	int i6 = VERTICES.size()-2;
	int i7 = VERTICES.size()-1;

	pl1 = Plane(i6, i7, i3, i2);
	pl2 = Plane(i2, i3, i1, i0);
	pl3 = Plane(i4, i6, i2, i0);
	pl4 = Plane(i1, i3, i7, i5);
	pl5 = Plane(i4, i5, i7, i6);
	pl6 = Plane(i0, i1, i5, i4);

	vps.push_back(&VERTICES[i0]);
	vps.push_back(&VERTICES[i1]);
	vps.push_back(&VERTICES[i2]);
	vps.push_back(&VERTICES[i3]);
	vps.push_back(&VERTICES[i4]);
	vps.push_back(&VERTICES[i5]);
	vps.push_back(&VERTICES[i6]);
	vps.push_back(&VERTICES[i7]);

	f1_index = FRAME.size();
	FRAME.push_back(i0);
	FRAME.push_back(i1);
	FRAME.push_back(i0);
	FRAME.push_back(i2);
	FRAME.push_back(i0);
	FRAME.push_back(i4);
	FRAME.push_back(i1);
	FRAME.push_back(i3);
	FRAME.push_back(i1);
	FRAME.push_back(i5);
	FRAME.push_back(i2);
	FRAME.push_back(i3);
	FRAME.push_back(i2);
	FRAME.push_back(i6);
	FRAME.push_back(i3);
	FRAME.push_back(i7);
	FRAME.push_back(i4);
	FRAME.push_back(i5);
	FRAME.push_back(i4);
	FRAME.push_back(i6);
	FRAME.push_back(i5);
	FRAME.push_back(i7);
	FRAME.push_back(i6);
	FRAME.push_back(i7);
	f2_index = FRAME.size()-1;
}
void Rpsm::v_inv() {

}

// dims: the location dims[0] and the dimensions dims[1]
// clr: the color of the field
// gen: the lambda that determines where you should be given the inputted values
// res: the resolution, or space between generated vertices
// Note - Usually best used when one of the dimensions is 0
Field::Field(glm::mat2x3 dims, glm::vec4 clr, int res, std::function<glm::vec3(glm::vec3)> gen) {
	glm::vec3 s = dims[0];
	glm::vec3 d = dims[1] + glm::vec3(1, 1, 1);

	get = gen;

	// Generating
	v1_index = VERTICES.size();
	f1_index = FRAME.size();
	for (int x = s.x; x < s.x + d.x; x += res)
		for (int y = s.y; y < s.y + d.y; y += res)
			for (int z = s.z; z < s.z + d.z; z += res) {
				VERTICES.push_back({ get({x, y, z}), {0, 0, 0}, clr });

				if (x > s.x) {
					FRAME.push_back(VERTICES.size()-1);
					FRAME.push_back(VERTICES.size()-1-d.y*d.z);
				}

				if (y > s.y) {
					FRAME.push_back(VERTICES.size()-1);
					FRAME.push_back(VERTICES.size()-1-d.z);
				}

				if (z > s.z) {
					FRAME.push_back(VERTICES.size()-1);
					FRAME.push_back(VERTICES.size()-2);
				}
			}
	v2_index = VERTICES.size()-1;
	f2_index = FRAME.size()-1;
	//
}
void Field::v_inv() {
	std::reverse(VERTICES.begin()+v1_index, VERTICES.begin()+v2_index);
}