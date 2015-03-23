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

		bool load_obj(bool debug);
		void render();
};

#endif
