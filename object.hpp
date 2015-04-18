#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include <string>
#include <stdint.h>
#include <string.h>
#include <iostream>

#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include "types.hpp"
#include "shaders.hpp"
#include "image.hpp"
#include "misc.hpp"

using namespace std;

struct Group{
	string texture_path;
	GLuint program_id, shadow_program_id, matrix_id, vertexposition_modelspace_id, vertexUV_id, 
	texture, texture_id, shadowmap_id, depthbias_id, shadow_matrix_id,
	vertexbuffer, uvbuffer, normalbuffer, size,
	viewmatrix_id, lightdir_id, normal_id;
};

class Object {

	public:
		string name, texture_path, obj_path;

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
		void render_ntexture();
		void render_texture();
		void set_shadowmap(GLuint dt);
		void render_shadow();
		
		void move(float _x, float _y, float _z);
		vector<Group> groups;
	private:
		bool load_obj_texture();
		bool load_obj_ntexture();
		bool debug=false;
		void load_debug(string path, vector<Point> &vertices, vector<Point> &normals, vector<Face> &faces, vector<Point> &uvs);
		Group load_group(string group_name);
		void get_depthbiasmvp(float dbmvp[4][4]);
		
		GLuint shadowmap;
		float depthMVP[4][4];
	protected:
		float x, y, z, s;
		string path;
		bool has_texture;
		void get_mvp(float mvp[4][4]);
};

#endif
