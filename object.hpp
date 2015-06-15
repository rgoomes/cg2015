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
		virtual void render_texture();
		virtual void render_glass(int* order);
		void attach_loader(Loader* loader);
		void attach_world(World* world);
		World* get_world();
		
		float x, y, z, s;
		void move(float _x, float _y, float _z);
		virtual void rotate(btVector3 up, float degree);
		virtual string type(){return "object";}
		virtual void set_transparent(bool is_t);
		bool is_transparent();
		btVector3 get_position();

		Model* model;
		bool bumpfix = false;

		virtual void sort_groups();
	private:

		bool debug=false;
		bool has_transparency=false;
		void set_material(Group& g, Material& m);
	protected:
		virtual void render_group(Group& g, float m[][4], float view[][4], float depthbias_mvp[][4]);
		bool load_obj_texture();
		Group load_group(string group_name);
		void get_matrix(float m[16]);
		Loader* loader = NULL;
		World* world = NULL;

		GLuint shadowmap;
		float depthMVP[4][4];
		void get_depthbiasmvp(float dbmvp[4][4]);
		string path;
		bool has_texture;
		int receives_shadows = 1;
		void get_mvp(float mvp[4][4]);
		virtual btTransform* get_transform();
		btTransform trans;
};

#endif
