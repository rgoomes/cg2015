#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include <string>
#include <GLFW/glfw3.h>

#include "types.hpp"
#include "shaders.hpp"
#include "image.hpp"

using namespace std;

struct Group{
	string texture_path;
	GLuint program_id, matrix_id, vertexposition_modelspace_id, vertexUV_id, texture, texture_id, vertexbuffer, uvbuffer, size;
};

class Object {

	public:
		string path, name, texture_path, obj_path;

		vector<Point> vertices, normals;
		vector<Point2> uvs;
		vector<Point> out_vertices, out_normals;
		vector<Point2> out_uvs;
		vector<Face> faces, all_faces;

		Object(string path);
		~Object();

		bool load_obj(bool texture);
		void set_scale(float s);
		float scale();
		void render();
		void render(float m[4][4]);
		
	private:
		bool load_obj_texture();
		bool load_obj_ntexture();
		float s;
		bool debug=false;
		void load_debug(string path, vector<Point> &vertices, vector<Point> &normals, vector<Face> &faces, vector<Point> &uvs);
		Group load_group(string group_name);
		
		vector<Group> groups;
};

#endif
