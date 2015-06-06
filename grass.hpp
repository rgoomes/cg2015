#ifndef GRASS_H
#define GRASS_H

#include <string>
#include "object.hpp"

using namespace std;

class Grass : public Object{
public:
	Grass(string path);
	Grass(string path, Loader* loader, btVector3 pos, double r);
	void render_texture();
	void render_glass();
private:
	string path;
	Loader* loader;
	GLuint texture;
	GLuint shader_id;
	double r;
};



#endif