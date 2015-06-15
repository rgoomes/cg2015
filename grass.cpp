
#include "grass.hpp"
#include <stdio.h>
#include <math.h>

Grass::Grass(string path, Loader* loader, btVector3 pos, double r)
: Object(path, loader){
	this->path = path;
	this->loader = loader;
	this->r = r;
	receives_shadows = 0;

	time = ((rand() % 1000) / 1000.0) * 3.14;
}

Grass::Grass(string path)
: Object(path){
	this->path = path;
	this->r = 1;
	receives_shadows = 0;

	time = ((rand() % 1000) / 1000.0) * 3.14;
}

void Grass::render_texture(){
	Object::render_texture();
}

void Grass::render_glass(int* order){
	glDisable(GL_CULL_FACE);
	Object::render_glass(this->order);
	glEnable(GL_CULL_FACE);
}

void Grass::sort_groups(){
	btVector3 v = world->camera->get_obs_pos() - get_position();
	v.normalize();
	btScalar angle = atan2(v.getX(), v.getZ()) / (1.0467) + 3;
	int left = (int)(6 - (int)angle) % 6, right;
	right = (left + 1) % 6;

	int f_pos[] = { (left-2 + 6)%6, (right+2)%6, (left-1 +6 )%6, (right+1)%6, left, right };

	order[0] = 0;
	for(int i=0; i<6; i++){
		if(f_pos[i] == 0)
			f_pos[i] = 6;
		order[i+1] = f_pos[i];
	}
}


void Grass::render_group(Group& g, float m[][4], float view[][4], float depthbias_mvp[][4]){
	
	time = world->timer->elapsed() + 0.5 * ((rand() % 1000) / 1000.0);

	for(int i=0; i<(signed)g.vertices.size(); i++){
		if(g.vertices[i].y > 0){
			g.vertices[i].x += 3 * cos(time);
			g.vertices[i].z += 2 * cos(0.5 + time * 0.5);
		}
	}

	glBindBuffer(GL_ARRAY_BUFFER, g.vertexbuffer);
	//glInvalidateBufferSubData(g.vertexbuffer, 0, g.vertices.size()*sizeof(Point));
	glBufferSubData(GL_ARRAY_BUFFER, 0, g.vertices.size()*sizeof(Point), g.vertices.data() );
	//void* r = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY_ARB);
	
	for(int i=0; i<(signed)g.vertices.size(); i++){
		if(g.vertices[i].y > 0){
			g.vertices[i].x -= 3 * cos(time);
			g.vertices[i].z -= 2 * cos(0.5 + time * 0.5);
		}
	}
	Object::render_group(g, m, view, depthbias_mvp);
}

bool Grass::load_obj(){
	bool t = Object::load_obj();
	for(int i=0; i<(signed)model->groups.size(); i++){
		Group& g = model->groups[i];
		glBindBuffer(GL_ARRAY_BUFFER, g.vertexbuffer);
		glBufferData(GL_ARRAY_BUFFER, g.vertices.size()*sizeof(Point), g.vertices.data(), GL_DYNAMIC_DRAW);
	}

	return t;
}