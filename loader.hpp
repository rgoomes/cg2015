#ifndef LOADER_H
#define LOADER_H

#include <map>
#include <vector>
#include <utility>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "image.hpp"
#include "shaders.hpp"

using namespace std;
using namespace glm;

struct Material{
	vec3 Kd, Ka, Ks;
	float Tf, Ns;
	int illum;
	string bump, map_Ka;
};

struct Group{
	string texture_path;
	GLuint  program_id, shadow_program_id, matrix_id, vertexposition_modelspace_id, vertexUV_id, 
			texture, texture_id, shadowmap_id, depthbias_id, shadow_matrix_id,
			vertexbuffer, uvbuffer, normalbuffer, size,
			viewmatrix_id, modelmatrix_id, lightdir_id, normal_id,
			has_texture_id;
	Material material;
	GLuint Ns_id, Tf_id;
};

struct Model{
	vector<Group> groups;
	vector<Face> all_faces;
	vector<Point> vertices;
};

class Loader{
	public:
		Loader();
		~Loader();
		GLuint get_texture(string path);
		GLuint get_shaders(string pathVertex, string pathFragment);
		Model get_model(string path, float scale);
	private:
		map<string, GLuint> textures;
		map<pair<string, string>, GLuint> shaders;
		map<string, Model> models;

		vector<Point> normals;
		vector<Point2> uvs;
		vector<Point> out_vertices, out_normals;
		vector<Point2> out_uvs;
		vector<Face> faces;
		vector<Face> all_faces;
		vector<Point> vertices;
		map<string, Material> materials;
		string texture_path, path, obj_path, name, mtl_path;
		string cur_material;
		float s;

		vector<Group> groups;

		Model load_model(string path);
		Group load_group(string group_name);
		void load_mtl(FILE* file);
		void init_material(Material& m);
		void set_material_ids(Group &g, Material& m);
};

#endif