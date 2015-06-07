
#include "grass.hpp"
#include <stdio.h>
#include <math.h>

Grass::Grass(string path, Loader* loader, btVector3 pos, double r)
: Object(path, loader){
	this->path = path;
	this->loader = loader;
	this->r = r;
}

Grass::Grass(string path)
: Object(path){
	this->path = path;
	this->r = 1;
}

void Grass::render_texture(){
	Object::render_texture();
}

void Grass::render_glass(){
	glDisable(GL_CULL_FACE);
	Object::render_glass();
	glEnable(GL_CULL_FACE);
}

void Grass::sort_groups(){
	btVector3 v = world->camera->get_obs_pos() - get_position();
	v.normalize();
	btScalar angle = atan2(v.getX(), v.getZ()) / (1.0467) + 3;
	//printf("%f %f\n", v.getX(), v.getZ());
	int left = (int)(6 - (int)angle) % 6, right;
	right = (left + 1) % 6;

	int f_pos[] = { (left-2 + 6)%6, (right+2)%6, (left-1 +6 )%6, (right+1)%6, left, right };

	vector<Group> n_groups;
	n_groups.resize(7);
	for(int i=0; i<6; i++){
		if(f_pos[i] == 0)
			f_pos[i] = 6;
		n_groups[i+1] = model->groups[f_pos[i]];
	}
	temp = model->groups;
	model->groups = n_groups;
}

void Grass::unsort_groups(){
	model->groups = temp;
}