#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include <string>
#include <stdint.h>
#include <string.h>
#include <iostream>

#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include "shaders.hpp"
#include "image.hpp"
#include "loader.hpp"
#include "world.hpp"

using namespace std;

class Object {

	public:
		string name, texture_path, obj_path;
		bool is_static = false;

		Object(string path);
		Object(string path, Loader* loader);
		~Object();

		bool load_obj();
		void set_scale(float s);
		float scale();
		void render();
		void set_shadowmap(GLuint dt);
		void render_shadow();
		void render_texture();
		void render_glass();
		void attach_loader(Loader* loader);
		void attach_world(World* world);
		
		float x, y, z, s;
		void move(float _x, float _y, float _z);
		virtual void rotate(btVector3 up, float degree);
		virtual string type(){return "object";}

		Model* model;
		bool bumpfix = false;
	private:

		bool load_obj_texture();
		bool debug=false;
		Group load_group(string group_name);
		Loader* loader = NULL;
		World* world = NULL;
		void get_matrix(float m[16]);
		void set_material(Group& g, Material& m);
	protected:

		GLuint shadowmap;
		float depthMVP[4][4];
		void get_depthbiasmvp(float dbmvp[4][4]);
		string path;
		bool has_texture;
		void get_mvp(float mvp[4][4]);
		virtual btTransform* get_transform();
		btTransform trans;
};

#endif
