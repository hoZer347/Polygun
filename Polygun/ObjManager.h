#pragma once

#include <vector>
#include <set>

#include "Object.h"
#include "Player.h"

class ObjManager {
public:
	ObjManager() {};
	ObjManager(Player*, Field*);

	void del(Object*);
	void update();
	void add_enemy(Enemy*);
	void add_proj();

	auto end() { return objs.end(); };
	auto begin() { return objs.begin(); };

	std::set<Object*> objs;

private:
	Player* p = NULL;
	Field* f = NULL;

	std::set<Prj*> prjs;
	std::set<Enemy*> enms;
	std::set<Object*> cull;
};

