
#include "grass.hpp"
#include <stdio.h>

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