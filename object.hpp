#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include <string>

#include "types.hpp"

using namespace std;

class Object {

	public:
		string path;

		vector<Point> vertices, normals, uvs;
		vector<Point> out_vertices, out_normals;
		vector<Face> faces;

		Object(string path);
		~Object();

		bool load_obj(bool texture, bool debug);
		bool load_obj(bool texture);
		void render();

	private:
		bool load_obj_texture(bool debug);
		bool load_obj_ntexture(bool debug);
};

#endif
