#include <stdint.h>
#include <string.h>
#include <iostream>

#include "object.hpp"

using namespace std;

Object::Object(string path){
	this->path = path;
	int l = this->path.find_last_of("/");
	this->name = this->path.substr(l+1, string::npos);
	this->texture_path = this->path + "/" + this->name + ".dds";
	this->obj_path = this->path + "/" + this->name + ".obj";
	std::cout << this->obj_path << "\n";
	this->s = 1.0;
}
Object::~Object(){}

void Object::render(){

}

void Object::render(float m[4][4]){

	glUseProgram(programID);
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &m[0][0]);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture);
	glUniform1i(TextureID, 0);

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(vertexPosition_modelspaceID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(
		vertexPosition_modelspaceID,  // The attribute we want to configure
		3,                            // size
		GL_FLOAT,                     // type
		GL_FALSE,                     // normalized?
		0,                            // stride
		(void*)0                      // array buffer offset
	);

	// 2nd attribute buffer : UVs
	glEnableVertexAttribArray(vertexUVID);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glVertexAttribPointer(
		vertexUVID,                   // The attribute we want to configure
		2,                            // size : U+V => 2
		GL_FLOAT,                     // type
		GL_FALSE,                     // normalized?
		0,                            // stride
		(void*)0                      // array buffer offset
	);

	// Draw the triangles !
	glDrawArrays(GL_TRIANGLES, 0, out_vertices.size()*3); // 12*3 indices starting at 0 -> 12 triangles

	glDisableVertexAttribArray(vertexPosition_modelspaceID);
	glDisableVertexAttribArray(vertexUVID);

}

void Object::set_scale(float s){
	this->s = s;
}

float Object::scale(){
	return s;
}

bool Object::load_obj(bool has_texture){
	if(has_texture)
		return load_obj_texture();
	else
		return load_obj_ntexture();
}

bool Object::load_obj_ntexture(){
	FILE *file = fopen(obj_path.c_str(), "r");
	if(!file)
		return false;

	int tmp;
	char line[128];
	while(fscanf(file, "%s", line) != EOF){
		Point coord;
		Point2 coord2;
		Face face;

		if(!strcmp(line, "v")){
			tmp = fscanf(file, "%f %f %f", &coord.x, &coord.y, &coord.z);
			coord.x *= s; coord.y *= s; coord.z *= s;
			this->vertices.push_back(coord);
		} else if(!strcmp(line, "vn")){
			tmp = fscanf(file, "%f %f %f", &coord.x, &coord.y, &coord.z);
			this->normals.push_back(coord);
		} else if(!strcmp(line, "vt")){
			tmp = fscanf(file, "%f %f", &coord2.x, &coord2.y);
			this->uvs.push_back(coord2);
		} else if(!strcmp(line, "f")){
			tmp = fscanf(file, "%d//%d %d//%d %d//%d", &face.v_index[0], &face.n_index[0],
			&face.v_index[1], &face.n_index[1], &face.v_index[2], &face.n_index[2]);
			this->faces.push_back(face);
		}

		if(tmp <= 0)
			return false;
	}

	for(uint64_t i = 0; i < this->faces.size(); i++){
		out_vertices.push_back(vertices[faces[i].v_index[0] - 1]);
		out_normals.push_back(normals  [faces[i].n_index[0] - 1]);
		out_vertices.push_back(vertices[faces[i].v_index[1] - 1]);
		out_normals.push_back(normals  [faces[i].n_index[1] - 1]);
		out_vertices.push_back(vertices[faces[i].v_index[2] - 1]);
		out_normals.push_back(normals  [faces[i].n_index[2] - 1]);
	}

	/*if(debug)
		load_debug(path, vertices, normals, faces, uvs);*/

	fclose(file);
	return true;
}


bool Object::load_obj_texture(){
	FILE *file = fopen(obj_path.c_str(), "r");
	if(!file)
		return false;

	int tmp;
	char line[128];
	while(fscanf(file, "%s", line) != EOF){
		Point coord;
		Point2 coord2;
		Face face;

		if(!strcmp(line, "v")){
			tmp = fscanf(file, "%f %f %f", &coord.x, &coord.y, &coord.z);
			coord.x *= s; coord.y *= s; coord.z *= s;
			this->vertices.push_back(coord);
		} else if(!strcmp(line, "vn")){
			tmp = fscanf(file, "%f %f %f", &coord.x, &coord.y, &coord.z);
			this->normals.push_back(coord);
		} else if(!strcmp(line, "vt")){
			tmp = fscanf(file, "%f %f", &coord2.x, &coord2.y);
			this->uvs.push_back(coord2);
		} else if(!strcmp(line, "f")){
			tmp = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d", &face.v_index[0], &face.t_index[0], &face.n_index[0],
			&face.v_index[1], &face.t_index[1], &face.n_index[1], &face.v_index[2], &face.t_index[2], &face.n_index[2]);

			this->faces.push_back(face);
		}

		if(tmp <= 0)
			return false;
	}

	for(uint64_t i = 0; i < this->faces.size(); i++){
		out_vertices.push_back(vertices	[faces[i].v_index[0] - 1]);
		out_normals.push_back(normals 	[faces[i].n_index[0] - 1]);
		out_uvs.push_back(uvs 			[faces[i].t_index[0] - 1]);
		out_vertices.push_back(vertices	[faces[i].v_index[1] - 1]);
		out_normals.push_back(normals 	[faces[i].n_index[1] - 1]);
		out_uvs.push_back(uvs 			[faces[i].t_index[1] - 1]);
		out_vertices.push_back(vertices	[faces[i].v_index[2] - 1]);
		out_normals.push_back(normals 	[faces[i].n_index[2] - 1]);
		out_uvs.push_back(uvs 			[faces[i].t_index[2] - 1]);
	}


	programID = load_shaders( "objects/textureVertexShader.glsl", "objects/textureFragmentShader.glsl" );
	MatrixID = glGetUniformLocation(programID, "MVP");

	vertexPosition_modelspaceID = glGetAttribLocation(programID, "vertexPosition_modelspace");
	vertexUVID = glGetAttribLocation(programID, "vertexUV");

	Texture = loadDDS(texture_path.c_str());
	TextureID  = glGetUniformLocation(programID, "myTextureSampler");

	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, out_vertices.size()*sizeof(Point), out_vertices.data(), GL_DYNAMIC_DRAW);
	
	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, out_uvs.size()*sizeof(Point2), out_uvs.data(), GL_DYNAMIC_DRAW);
	
	/*if(debug)
		load_debug(path, vertices, normals, faces, uvs);*/

	fclose(file);
	return true;
}

void load_debug(string path, vector<Point> &vertices, vector<Point> &normals, vector<Face> &faces, vector<Point> &uvs){
		std::cout << "Loading object: " << path 	 << "\n"
				  << "Vertices: " << vertices.size() << "\t"
				  << "Normals: "  << normals.size()  << "\t"
				  << "Faces: "    << faces.size() 	 << "\t"
				  << "Uvs: "      << uvs.size() 	 << "\n\n";
}