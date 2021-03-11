#pragma once

#include "glfw3.h"
#include "glm.hpp"
#include <vector>

// Placeholder values for vertices
static int DEFAULT_X = 0;
static int DEFAULT_Y = 1;
static int DEFAULT_Z = 2;
static int DEFAULT_R = 0;
static int DEFAULT_G = 0;
static int DEFAULT_B = 0;
static int DEFAULT_A = 0;
static glm::vec4 DEFAULT_COLOR = { 0, 0, 0, 0};

extern std::vector<GLfloat> VERTICES;
extern std::vector<GLfloat> FRAME;
extern std::vector<GLfloat> COLOR;
//

// Vertex
// Functions as a position (x, y, z) and a color (r, g, b, a)
class Vtx {
public:
	Vtx() {};
	void load(glm::vec3, glm::vec4={ 1, 1, 1, 1 });

private:
	int& x = DEFAULT_X;
	int& y = DEFAULT_Y;
	int& z = DEFAULT_Z;
	int& r = DEFAULT_R;
	int& g = DEFAULT_G;
	int& b = DEFAULT_B;
	int& a = DEFAULT_A;
};

class Line {
public:
	Line() {};
	void load(glm::mat3x2, glm::vec4={ 1, 1, 1, 1 });

private:
	int& x1 = DEFAULT_X;
	int& y1 = DEFAULT_Y;
	int& z1 = DEFAULT_Z;
	int& x2 = DEFAULT_X;
	int& y2 = DEFAULT_Y;
	int& z2 = DEFAULT_Z;
};

// Triangle
// Functions as 3 vertices
class Tri {
public:
	Tri() {};
	void load(glm::mat3, glm::mat4x3=glm::mat4x3(1));

private:
	Vtx v1;
	Vtx v2;
	Vtx v3;
};

// Plane
// Functions as a plane between 2 points
class Plane {
public:
	Plane() {};
	void load(glm::vec3, glm::vec3);

private:
	Tri t1;
	Tri t2;
};

// Rectangular prism
// Functions as 6 planes
class Rpsm {
public:
	Rpsm() {};
	void load(glm::vec3, glm::vec3);

private:
	// Positions are relative to up=(0, 1, 0)
	Plane top;
	Plane bot;
	Plane frt;
	Plane bck;
	Plane lft;
	Plane rgt;
};