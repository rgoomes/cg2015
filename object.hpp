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


class Object {

	public:
		string name, texture_path, obj_path;

		Object(string path);
		Object(string path, Loader* loader);
		~Object();

		bool load_obj(bool texture);
		void set_scale(float s);
		float scale();
		void render();
		void set_shadowmap(GLuint dt);
		void render_shadow();
		void render_texture();
		void render_glass();
		void attach_loader(Loader* loader);
		void attach_world(World* world);
		
		void move(float _x, float _y, float _z);
		virtual string type(){return "object";}

	private:

		bool load_obj_texture();
		bool debug=false;
		Group load_group(string group_name);
		Loader* loader = NULL;
		World* world = NULL;
		void get_matrix(float m[16]);
		void set_material(Group& g, Material& m);
	protected:
		Model* model;

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
