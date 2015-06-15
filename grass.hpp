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
	void render_glass(int* order);
	bool load_obj();

	void sort_groups();
	void unsort_groups();

	int order[7] = {0, 1, 2, 3, 4, 5, 6};
	double time=0;
private:
	string path;
	Loader* loader;
	GLuint texture;
	GLuint shader_id;
	double r;

	vector<Group> temp;
protected:
	void render_group(Group& g, float m[][4], float view[][4], float depthbias_mvp[][4]);
};



#endif