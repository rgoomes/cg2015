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
	x=0; y=0; z=0;
}
Object::~Object(){}

void Object::render(){
	glUseProgram(0);

	for(int i = 0; i < (int)all_faces.size(); i++){
		int v, n;
		glBegin(GL_TRIANGLES);
			v = all_faces[i].v_index[2]-1;
			n = all_faces[i].n_index[2]-1;
			glNormal3f(normals[n].x, normals[n].y, normals[n].z);
			glVertex3f(vertices[v].x, vertices[v].y, vertices[v].z);

			v = all_faces[i].v_index[1]-1;
			n = all_faces[i].n_index[1]-1;
			glNormal3f(normals[n].x, normals[n].y, normals[n].z);
			glVertex3f(vertices[v].x, vertices[v].y, vertices[v].z);

			v = all_faces[i].v_index[0]-1;
			n = all_faces[i].n_index[0]-1;
			glNormal3f(normals[n].x, normals[n].y, normals[n].z);
			glVertex3f(vertices[v].x, vertices[v].y, vertices[v].z);
			
		glEnd();
	}	
}

void Object::move(float _x, float _y, float _z){
	x = _x;
	y = _y;
	z = _z;
}

void Object::render(float m[4][4]){

	for(int i=0; i<(int)groups.size(); i++){
		Group g = groups[i];

		glUseProgram(g.program_id);
		glUniformMatrix4fv(g.matrix_id, 1, GL_FALSE, &m[0][0]);
		float translate_array[] = {x, y, z};
		glUniform3fv(g.translate_id, 1, translate_array);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, g.texture);
		glUniform1i(g.texture_id, 0);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(g.vertexposition_modelspace_id);
		glBindBuffer(GL_ARRAY_BUFFER, g.vertexbuffer);
		glVertexAttribPointer(
			g.vertexposition_modelspace_id,  // The attribute we want to configure
			3,                            // size
			GL_FLOAT,                     // type
			GL_FALSE,                     // normalized?
			0,                            // stride
			(void*)0                      // array buffer offset
		);
		
		// 2nd attribute buffer : UVs
		glEnableVertexAttribArray(g.vertexUV_id);
		glBindBuffer(GL_ARRAY_BUFFER, g.uvbuffer);
		glVertexAttribPointer(
			g.vertexUV_id,                // The attribute we want to configure
			2,                            // size : U+V => 2
			GL_FLOAT,                     // type
			GL_FALSE,                     // normalized?
			0,                            // stride
			(void*)0                      // array buffer offset
		);

		// Draw the triangles !
		glDrawArrays(GL_TRIANGLES, 0, g.size*3); // 12*3 indices starting at 0 -> 12 triangles

		glDisableVertexAttribArray(g.vertexposition_modelspace_id);
		glDisableVertexAttribArray(g.vertexUV_id);
		
	}


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

	int tmp=1;
	char line[256], *tmp_c;
	char buffer[1000];
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
		} else if(!strcmp(line, "#")){
			tmp_c = fgets(buffer, 1000, file);
			if(tmp_c == NULL) return false;
			//printf("Comment: %s\n", buffer);
		}

		if(tmp <= 0)
			return false;
	}

	for(uint64_t i = 0; i < this->faces.size(); i++){
		printf("%ld\n", i);
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

Group Object::load_group(string group_name){
	out_vertices.clear();
	out_normals.clear();
	out_uvs.clear();
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

	Group g;

	g.program_id = load_shaders( "objects/textureVertexShader.glsl", "objects/textureFragmentShader.glsl" );
	g.matrix_id = glGetUniformLocation(g.program_id, "MVP");
	g.translate_id = glGetUniformLocation(g.program_id, "translate");

	g.vertexposition_modelspace_id = glGetAttribLocation(g.program_id, "vertexPosition_modelspace");
	g.vertexUV_id = glGetAttribLocation(g.program_id, "vertexUV");

	g.size = out_vertices.size();
	g.texture = loadDDS(texture_path.c_str());
	g.texture_id  = glGetUniformLocation(g.program_id, "mytextureSampler");
	texture_path = path + "/" + group_name + ".dds";
	
	glGenBuffers(1, &g.vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, g.vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, out_vertices.size()*sizeof(Point), out_vertices.data(), GL_DYNAMIC_DRAW);
	
	glGenBuffers(1, &g.uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, g.uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, out_uvs.size()*sizeof(Point2), out_uvs.data(), GL_DYNAMIC_DRAW);

	all_faces.insert(all_faces.end(),faces.begin(),faces.end());

	faces.clear();
	return g;
}

bool Object::load_obj_texture(){
	FILE *file = fopen(obj_path.c_str(), "r");
	if(!file)
		return false;

	int tmp=1;
	char buffer[1000];
	char line[256], *tmp_c;
	string texture_path;
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
		}else if(!strcmp(line, "#")){
			tmp_c = fgets(buffer, 1000, file);
			if(tmp_c == NULL) return false;
			//printf("Comment: %s\n", buffer);
		}else if(!strcmp(line, "g")){
			char group_name[30];
			tmp = fscanf(file, "%s", group_name);
			
			Group g = load_group(group_name);

			groups.push_back(g);
		}

		if(tmp <= 0)
			return false;
	}

	Group g = load_group("");
	groups.push_back(g);
	printf("Loaded %s\n", obj_path.c_str());
	
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