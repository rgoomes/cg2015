
#include "grass.hpp"

Grass::Grass(string path, Loader* loader, btVector3 pos, double r)
: Object(path, loader){
	this->path = path;
	this->loader = loader;
	this->r = r;
}

void Grass::load(){
	texture = loader->get_texture(path);
	shader_id = loader->get_shaders("grassVertexShader.glsl", "grassFragmentShader.glsl");
}

void Grass::render(){

}