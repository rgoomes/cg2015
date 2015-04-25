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
#include "loader.hpp"
#include "world.hpp"

using namespace std;


struct Group{
	string texture_path;
	GLuint program_id, shadow_program_id, matrix_id, vertexposition_modelspace_id, vertexUV_id, 
	texture, texture_id, shadowmap_id, depthbias_id, shadow_matrix_id,
	vertexbuffer, uvbuffer, normalbuffer, size,
	viewmatrix_id, modelmatrix_id, lightdir_id, normal_id;
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
		Object(string path, Loader* loader);
		~Object();

		bool load_obj(bool texture);
		void set_scale(float s);
		float scale();
		void render();
		void render_ntexture();
		void render_texture();
		void set_shadowmap(GLuint dt);
		void render_shadow();
		void attach_loader(Loader* loader);
		void attach_world(World* world);
		
		void move(float _x, float _y, float _z);
		vector<Group> groups;
		virtual string type(){return "object";}
	private:
		bool load_obj_texture();
		bool debug=false;
		void load_debug(string path, vector<Point> &vertices, vector<Point> &normals, vector<Face> &faces, vector<Point> &uvs);
		Group load_group(string group_name);
		Loader* loader = NULL;
		World* world = NULL;
		void get_matrix(float m[16]);
	protected:
		GLuint shadowmap;
		float depthMVP[4][4];
		void get_depthbiasmvp(float dbmvp[4][4]);
		float x, y, z, s;
		string path;
		bool has_texture;
		void get_mvp(float mvp[4][4]);
		virtual btTransform* get_transform();
		btTransform trans;
};

#endif
