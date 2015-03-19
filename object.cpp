#include <stdint.h>
#include <string.h>
#include <iostream>

#include "object.hpp"

using namespace std;

Object::Object(string path){ this->path = path; }
Object::~Object(){}

bool Object::load_obj(bool debug){

	FILE *file = fopen(this->path.c_str(), "r");
	if(!file)
		return false;

	char line[128];
	while(fscanf(file, "%s", line) != EOF){
		Point coord;
		Face face; 

		if(!strcmp(line, "v")){
			fscanf(file, "%f %f %f", &coord.x, &coord.y, &coord.z);
			this->vertices.push_back(coord);
		} else if(!strcmp(line, "vn")){
			fscanf(file, "%f %f %f", &coord.x, &coord.y, &coord.z);
			this->normals.push_back(coord);
		} else if(!strcmp(line, "vt")){
			fscanf(file, "%f %f", &coord.x, &coord.y);
			this->uvs.push_back(coord);
		} else if(!strcmp(line, "f")){
			fscanf(file, "%d//%d %d//%d %d//%d", &face.v_index[0], &face.n_index[0],
			&face.v_index[1],  &face.n_index[1], &face.v_index[2], &face.n_index[2]);

			this->faces.push_back(face);
		}
	}

	for(uint64_t i = 0; i < this->faces.size(); i++){
		out_vertices.push_back(vertices[faces[i].v_index[0] - 1]);
		out_vertices.push_back(vertices[faces[i].v_index[1] - 1]);
		out_vertices.push_back(vertices[faces[i].v_index[2] - 1]);
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
