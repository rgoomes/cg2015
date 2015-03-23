#include <stdint.h>
#include <string.h>
#include <iostream>

#include "object.hpp"

using namespace std;

Object::Object(string path){
	this->path = path;
	this->s = 1.0;
}
Object::~Object(){}

void Object::render(){
	for(int i = 0; i < (int)out_vertices.size(); i += 3){
		glBegin(GL_TRIANGLES);
			glNormal3f(out_normals[i+2].x, out_normals[i+2].y, out_normals[i+2].z);
			glVertex3f(out_vertices[i+2].x, out_vertices[i+2].y, out_vertices[i+2].z);
			glNormal3f(out_normals[i+1].x, out_normals[i+1].y, out_normals[i+1].z);
			glVertex3f(out_vertices[i+1].x, out_vertices[i+1].y, out_vertices[i+1].z);
			glNormal3f(out_normals[i+0].x, out_normals[i+0].y, out_normals[i+0].z);
			glVertex3f(out_vertices[i+0].x, out_vertices[i+0].y, out_vertices[i+0].z);
			
		glEnd();
	}
}

void Object::set_scale(float s){
	this->s = s;
}

float Object::scale(){
	return s;
}

bool Object::load_obj(bool has_texture, bool debug){
	if(has_texture)
		return load_obj_texture(debug);
	else
		return load_obj_ntexture(debug);
}

bool Object::load_obj(bool has_texture){
	if(has_texture)
		return load_obj_texture(false);
	else
		return load_obj_ntexture(false);
}

bool Object::load_obj_ntexture(bool debug){
	FILE *file = fopen(this->path.c_str(), "r");
	if(!file)
		return false;

	char line[128];
	while(fscanf(file, "%s", line) != EOF){
		Point coord;
		Face face; 

		if(!strcmp(line, "v")){
			fscanf(file, "%f %f %f", &coord.x, &coord.y, &coord.z);
			coord.x *= s; coord.y *= s; coord.z *= s;
			this->vertices.push_back(coord);
		} else if(!strcmp(line, "vn")){
			fscanf(file, "%f %f %f", &coord.x, &coord.y, &coord.z);
			this->normals.push_back(coord);
		} else if(!strcmp(line, "vt")){
			fscanf(file, "%f %f", &coord.x, &coord.y);
			this->uvs.push_back(coord);
		} else if(!strcmp(line, "f")){
			fscanf(file, "%d//%d %d//%d %d//%d", &face.v_index[0], &face.n_index[0],
			&face.v_index[1], &face.n_index[1], &face.v_index[2], &face.n_index[2]);
			this->faces.push_back(face);
		}
	}

	for(uint64_t i = 0; i < this->faces.size(); i++){
		out_vertices.push_back(vertices[faces[i].v_index[0] - 1]);
		out_normals.push_back(normals  [faces[i].n_index[0] - 1]);
		out_vertices.push_back(vertices[faces[i].v_index[1] - 1]);
		out_normals.push_back(normals  [faces[i].n_index[1] - 1]);
		out_vertices.push_back(vertices[faces[i].v_index[2] - 1]);
		out_normals.push_back(normals  [faces[i].n_index[2] - 1]);
	}

	if(debug){
		std::cout << "Loading object: " << path 	 << "\n"
				  << "Vertices: " << vertices.size() << "\t"
				  << "Normals: "  << normals.size()  << "\t"
				  << "Faces: "    << faces.size() 	 << "\t"
				  << "Uvs: "      << uvs.size() 	 << "\n\n";
	}

	return true;
}

bool Object::load_obj_texture(bool debug){

	FILE *file = fopen(this->path.c_str(), "r");
	if(!file)
		return false;

	char line[128];
	while(fscanf(file, "%s", line) != EOF){
		Point coord;
		Face face; 

		if(!strcmp(line, "v")){
			fscanf(file, "%f %f %f", &coord.x, &coord.y, &coord.z);
			coord.x *= s; coord.y *= s; coord.z *= s;
			this->vertices.push_back(coord);
		} else if(!strcmp(line, "vn")){
			fscanf(file, "%f %f %f", &coord.x, &coord.y, &coord.z);
			this->normals.push_back(coord);
		} else if(!strcmp(line, "vt")){
			fscanf(file, "%f %f", &coord.x, &coord.y);
			this->uvs.push_back(coord);
		} else if(!strcmp(line, "f")){
			fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d", &face.v_index[0], &face.t_index[0], &face.n_index[0],
			&face.v_index[1], &face.t_index[1], &face.n_index[1], &face.v_index[2], &face.t_index[2], &face.n_index[2]);

			this->faces.push_back(face);
		}
	}

	for(uint64_t i = 0; i < this->faces.size(); i++){
		out_vertices.push_back(vertices[faces[i].v_index[0] - 1]);
		out_normals.push_back(normals  [faces[i].n_index[0] - 1]);
		out_vertices.push_back(vertices[faces[i].v_index[1] - 1]);
		out_normals.push_back(normals  [faces[i].n_index[1] - 1]);
		out_vertices.push_back(vertices[faces[i].v_index[2] - 1]);
		out_normals.push_back(normals  [faces[i].n_index[2] - 1]);
	}

	if(debug){
		std::cout << "Loading object: " << path 	 << "\n"
				  << "Vertices: " << vertices.size() << "\t"
				  << "Normals: "  << normals.size()  << "\t"
				  << "Faces: "    << faces.size() 	 << "\t"
				  << "Uvs: "      << uvs.size() 	 << "\n\n";
	}

	return true;
}
