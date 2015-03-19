#include "object.hpp"

Object::Object(string path){ this->path = path; }
Object::~Object(){}

bool Object::load_obj(bool debug){

	FILE *file = fopen(this->path.c_str(), "r");
	if(!file)
		return false;

	char line[128];
	printf("RETARDE\n");
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
			&face.v_index[1], &face.n_index[1], &face.v_index[2], &face.n_index[2]);

			this->faces.push_back(face);
		}
	}

	for( unsigned int i=0; i<this->faces.size(); i++ ){
		unsigned int ind;
		ind = this->faces[i].v_index[0];
		out_vertices.push_back(this->vertices[ind-1]);
		ind = this->faces[i].v_index[1];
		out_vertices.push_back(this->vertices[ind-1]);
		ind = this->faces[i].v_index[2];
		out_vertices.push_back(this->vertices[ind-1]);
		printf("RETARDE\n");
	}

	if(debug){
		std::cout << "Object:  " << this->path 		<< std::endl
				  << "Vertices " << vertices.size() << std::endl
				  << "Normals  " << normals.size()  << std::endl
				  << "Faces    " << faces.size() 	<< std::endl
				  << "Uvs      " << uvs.size() 		<< std::endl;
	}

	return true;
}

