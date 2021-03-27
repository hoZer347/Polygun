#include "ObjManager.h"

#include <iostream>

ObjManager::ObjManager(Camera* c, Field* field) {
	cam = c;
	f = field;
}

void ObjManager::del(Object* o) {
	Rpsm* g = &o->hit_box;

	int vd = g->v2_index-g->v1_index+1;
	int id = g->i2_index-g->i1_index+1;
	int fd = g->f2_index-g->f1_index+1;

	VERTICES.erase(VERTICES.begin()+g->v1_index, VERTICES.begin()+g->v2_index+1);
	INDICES.erase(INDICES.begin()+g->i1_index, INDICES.begin()+g->i2_index+1);
	FRAME.erase(FRAME.begin()+g->f1_index, FRAME.begin()+g->f2_index+1);

	for (auto& i : INDICES)
		if (i > g->v2_index)
			i -= vd;
	for (auto& i : FRAME)
		if (i > g->v2_index)
			i -= vd;

	int i1 = 0, i2 = 0, i3 = 0, i4 = 0;

	objs.erase(o);
	prjs.erase((Prj*)o);
	enms.erase((Enemy*)o);

	for (auto& i : objs) {
		if (i->hit_box.v1_index > g->v2_index) {
			i->hit_box.v1_index -= vd;
			i->hit_box.v2_index -= vd;
		}
		if (i->hit_box.i1_index > g->i2_index) {
			i->hit_box.i1_index -= id;
			i->hit_box.i2_index -= id;
		}
		if (i->hit_box.f1_index > g->f2_index) {
			i->hit_box.f1_index -= fd;
			i->hit_box.f2_index -= fd;
		}
	}
}

void ObjManager::update() {
	cull.clear();

	f->update();
	
	tst_i += 0.01f;

	for (auto e : enms) {
		f->get(e->pos);
		for (auto p : prjs) {
			if (p->hit_box.collide(e->hit_box)) {
				cull.insert(e);
				cull.insert(p);
			}
		}
	}

	for (auto& o : objs) {
		if (o->age > o->max_age) { cull.insert(o); continue; }
		o->update();
	}

	for (auto i : cull)
		del(i);
}

void ObjManager::add_enemy(Enemy* e) {
	objs.insert(e);
	enms.insert(e);
}

void ObjManager::add_proj() {
	glm::vec3 v1, v2;
	cam->get_pos(v1, v2);

	Prj* prj = new Prj(v1, (v2 - v1) /= 10);

	objs.insert(prj);
	prjs.insert(prj);
}
