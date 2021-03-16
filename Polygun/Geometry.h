#pragma once

#define GLM_FORCE_RADIANS

#include "glew.h"
#include "glfw3.h"
#include "glm.hpp"
#include "gtx/rotate_vector.hpp"

#include <stdio.h>
#include <algorithm>
#include <vector>
#include <functional>

struct Vtx;

extern std::vector<Vtx> VERTICES;
extern std::vector<GLint> INDICES;
extern std::vector<GLint> FRAME;

// Vertex
// Basic Data structure for building shapes
struct Vtx {
	glm::vec3 pos = glm::vec3(0);
	glm::vec3 nrm = glm::vec3(1);
	glm::vec4 clr = glm::vec4(1);
};

// Geometry
// Parent class for geometric shapes
// Geometries will usually have 2 ways of generation:
// - Default: Empty class with unassigned variables
// - Normal Constructor: Unique to each geometry, generates new vertices (parent shapes use this)
// - mk_ind Constructor: Generates as indices to existing vertices (sub-shapes use this), takes in indices as ints
class Geometry {
public:
	void rm_v();				// Automatically deletes vertex, index, and frame data
	virtual void v_inv()=0;		// Inverts the vertex order to invert normals
	void operator+=(glm::vec3);	// Translation
	void operator*=(GLfloat);	// Scaling


	int v1_index=0, v2_index=0;
	int i1_index = 0, i2_index = 0;
	int f1_index=0, f2_index=0;
};

// Tri
// Contains 3 vertices
class Tri : public Geometry {
public:
	friend class Plane;
	Tri() {};
	Tri(glm::mat3, glm::mat3x4);
	Tri(int, int, int);
	void mk_ind(int, int, int);
	void v_inv();
};

// Plane
// Contains 2 Triangles
class Plane : public Geometry {
public:
	friend class Rpsm;
	friend class Field;
	Plane() {};
	Plane(glm::mat4x3, glm::mat4);
	Plane(int, int, int, int);
	void mk_ind(int, int, int, int);
	void v_inv();
protected:
	Tri tri1;
	Tri tri2;
};

// Rectangular Prism
// Contains 6 outwards facing planes
class Rpsm : public Geometry {
public:
	Rpsm() {};
	Rpsm(glm::mat2x3, glm::vec4);
	void v_inv();

protected:
	Plane pl1;
	Plane pl2;
	Plane pl3;
	Plane pl4;
	Plane pl5;
	Plane pl6;
};

// Default field generation function, returns the input position
const static std::function<void(glm::vec3&)> def = [](glm::vec3& v) { return v; };
const static std::function<void(glm::vec3&)> tst = [](glm::vec3& v) {	
	float x = v.x;
	float y = (cos(v.x) + sin(v.z) + v.y) / 4;
	float z = v.z;

	v = glm::vec3(x, y, z);
};

// Field
// Contains a bunch of points representing a 3 variable function (get)
class Field : public Geometry {
public:
	Field() {};
	Field(glm::mat2x3, glm::vec4, int=1, std::function<void(glm::vec3&)> =def);
	void v_inv();

	std::function<void(glm::vec3&)> get;

private:
	int x_size=0, y_size=0, z_size=0;
};
