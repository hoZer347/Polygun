#pragma once

#include <vector>
#include <set>

#include "Object.h"
#include "Camera.h"

class ObjManager {
public:
	ObjManager() {};
	ObjManager(Camera* c, Field*);

	void del(Object*);
	void update();
	void add_enemy(Enemy*);
	void add_proj();

	auto end() { return objs.end(); };
	auto begin() { return objs.begin(); };

	std::set<Object*> objs;
	Camera* cam=NULL;

private:
	Field* f = NULL;

	std::set<Prj*> prjs;
	std::set<Enemy*> enms;
	std::set<Object*> cull;
};

