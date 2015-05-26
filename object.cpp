#include <stdint.h>
#include <string.h>
#include <iostream>

#include "object.hpp"

using namespace std;

Object::Object(string path, Loader* loader)
: Object(path){
	this->loader = loader;
}

Object::Object(string path){
	this->path = path;
	int l = this->path.find_last_of("/");
	this->name = this->path.substr(l+1, string::npos);
	this->texture_path = this->path + "/" + this->name + ".dds";
	this->obj_path = this->path + "/" + this->name + ".obj";
	this->s = 1.0;
	x=0; y=0; z=0;
}

void Object::attach_loader(Loader* loader){
	this->loader = loader;
}

void Object::attach_world(World* world){
	this->world = world;
}

Object::~Object(){}

void Object::render(){
	if(has_texture)
		render_texture();
}

/*void Object::render_ntexture(){
	glUseProgram(0);

	for(int i = 0; i < (int)all_faces.size(); i++){
		int v, n;
		glBegin(GL_TRIANGLES);
			v = all_faces[i].v_index[0]-1;
			n = all_faces[i].n_index[0]-1;
			glNormal3f(normals[n].x, normals[n].y, normals[n].z);
			glVertex3f(vertices[v].x, vertices[v].y, vertices[v].z);

			v = all_faces[i].v_index[1]-1;
			n = all_faces[i].n_index[1]-1;
			glNormal3f(normals[n].x, normals[n].y, normals[n].z);
			glVertex3f(vertices[v].x, vertices[v].y, vertices[v].z);

			v = all_faces[i].v_index[2]-1;
			n = all_faces[i].n_index[2]-1;
			glNormal3f(normals[n].x, normals[n].y, normals[n].z);
			glVertex3f(vertices[v].x, vertices[v].y, vertices[v].z);
			
		glEnd();
	}
}*/

void Object::move(float _x, float _y, float _z){
	x = _x;
	y = _y;
	z = _z;
}

void Object::rotate(btVector3 up, float degree){
	btTransform *trans = get_transform();
	btQuaternion q2 = trans->getRotation(), q;
	q.setRotation(up, degree / 180.0 * PI);
	q = q2 * q;
	trans->setRotation(q);
	this->trans = *trans;
}

void Object::get_depthbiasmvp(float dbmvp[4][4]){
	
	float bias[4][4] = {{0.5, 0.0, 0.0, 0.0}, 
						{0.0, 0.5, 0.0, 0.0},
						{0.0, 0.0, 0.5, 0.0},
						{0.5, 0.5, 0.5, 1.0}};
	mult_matrix(dbmvp, depthMVP, bias);
	
}

void Object::set_material(Group& g, Material& m){
	glUniform1f(g.Ns_id, m.Ns);
	glUniform1f(g.Tf_id, m.Tf);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m.map_Kd);
	glUniform1i(g.texture_id, 0);
	if(!g.material.map_Kd)
		glUniform1i(g.has_texture_id, 0);
	else
		glUniform1i(g.has_texture_id, 1);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, shadowmap);
	glUniform1i(g.shadowmap_id, 1);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m.bump);
	glUniform1i(g.bump_id, 2);
	if(!g.material.bump)
		glUniform1i(g.has_bump_id, 0);
	else
		glUniform1i(g.has_bump_id, 1);

}

void Object::render_texture(){
	float m[4][4];
	float depthbias_mvp[4][4];
	get_depthbiasmvp(depthbias_mvp);

	float view[4][4], model[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, view[0]);
	get_matrix(model);

	glPushMatrix();
		glMultMatrixf(model);
		get_mvp(m);
		
	glPopMatrix();

	for(int i=0; i<(int)this->model->groups.size(); i++){
		Group& g = this->model->groups[i];
		if(g.material.Tf < 1)
			continue;

		glUseProgram(g.program_id);
		glUniformMatrix4fv(g.matrix_id, 1, GL_FALSE, &m[0][0]);
		glUniformMatrix4fv(g.depthbias_id, 1, GL_FALSE, &depthbias_mvp[0][0]);
		glUniformMatrix4fv(g.viewmatrix_id, 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(g.modelmatrix_id, 1, GL_FALSE, model);
		glUniform3f(g.lightdir_id, -0.5, 0.8, 0.5);
		
		set_material(g, g.material);


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

		// 3rd attribute buffer : normals
		glEnableVertexAttribArray(g.normal_id);
		glBindBuffer(GL_ARRAY_BUFFER, g.normalbuffer);
		glVertexAttribPointer(
			g.normal_id,				  // The attribute we want to configure
			3,                            // size
			GL_FLOAT,                     // type
			GL_FALSE,                     // normalized?
			0,                            // stride
			(void*)0                      // array buffer offset
		);
		// 4th attribute buffer : tangents
		glEnableVertexAttribArray(g.tangent_id);
		glBindBuffer(GL_ARRAY_BUFFER, g.tangentbuffer);
		glVertexAttribPointer(
			g.tangent_id,				  // The attribute we want to configure
			3,                            // size
			GL_FLOAT,                     // type
			GL_FALSE,                     // normalized?
			0,                            // stride
			(void*)0                      // array buffer offset
		);

		// 5th attribute buffer : bitangents
		glEnableVertexAttribArray(g.bitangent_id);
		glBindBuffer(GL_ARRAY_BUFFER, g.bitangentbuffer);
		glVertexAttribPointer(
			g.bitangent_id,				  // The attribute we want to configure
			3,                            // size
			GL_FLOAT,                     // type
			GL_FALSE,                     // normalized?
			0,                            // stride
			(void*)0                      // array buffer offset
		);

		// Draw the triangles !
		glDrawArrays(GL_TRIANGLES, 0, g.size*3); // 12*3 indices starting at 0 -> 12 triangles

		glDisableVertexAttribArray(g.vertexposition_modelspace_id);
		glDisableVertexAttribArray(g.vertexUV_id);
		
		glDisable(GL_TEXTURE_2D);
	}


}

void Object::render_glass(){
	float m[4][4];
	float depthbias_mvp[4][4];
	get_depthbiasmvp(depthbias_mvp);

	float view[4][4], model[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, view[0]);
	get_matrix(model);

	glPushMatrix();
		glMultMatrixf(model);
		get_mvp(m);
		
	glPopMatrix();

	for(int i=0; i<(int)this->model->groups.size(); i++){
		Group& g = this->model->groups[i];
		if(g.material.Tf == 1)
			continue;

		glUseProgram(g.program_id);
		glUniformMatrix4fv(g.matrix_id, 1, GL_FALSE, &m[0][0]);
		glUniformMatrix4fv(g.depthbias_id, 1, GL_FALSE, &depthbias_mvp[0][0]);
		glUniformMatrix4fv(g.viewmatrix_id, 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(g.modelmatrix_id, 1, GL_FALSE, model);
		glUniform3f(g.lightdir_id, -0.5, 0.8, 0.5);
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, g.texture);
		glUniform1i(g.texture_id, 0);
		if(!g.material.map_Kd)
			glUniform1i(g.has_texture_id, 0);
		else
			glUniform1i(g.has_texture_id, 1);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, shadowmap);
		glUniform1i(g.shadowmap_id, 1);

		set_material(g, g.material);

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

		// 3rd attribute buffer : normals
		glEnableVertexAttribArray(g.normal_id);
		glBindBuffer(GL_ARRAY_BUFFER, g.normalbuffer);
		glVertexAttribPointer(
			g.normal_id,				  // The attribute we want to configure
			3,                            // size
			GL_FLOAT,                     // type
			GL_FALSE,                     // normalized?
			0,                            // stride
			(void*)0                      // array buffer offset
		);

		// Draw the triangles !
		glDrawArrays(GL_TRIANGLES, 0, g.size*3); // 12*3 indices starting at 0 -> 12 triangles

		glDisableVertexAttribArray(g.vertexposition_modelspace_id);
		glDisableVertexAttribArray(g.vertexUV_id);
		
		glDisable(GL_TEXTURE_2D);
	}


}


void Object::render_shadow(){
	float mt[16];
	glPushMatrix();
	get_matrix(mt);
	glMultMatrixf(mt);

	get_mvp(this->depthMVP);

	for(int i=0; i<(int)this->model->groups.size(); i++){
		Group& g = this->model->groups[i];

		glUseProgram(g.shadow_program_id);
		glUniformMatrix4fv(g.shadow_matrix_id, 1, GL_FALSE, &depthMVP[0][0]);

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

		// Draw the triangles !
		glDrawArrays(GL_TRIANGLES, 0, g.size*3); // 12*3 indices starting at 0 -> 12 triangles

		glDisableVertexAttribArray(g.vertexposition_modelspace_id);
		glDisableVertexAttribArray(g.vertexUV_id);
		
	}	
		
	glPopMatrix();
}

void Object::set_shadowmap(GLuint dt){
	shadowmap = dt;
}

void Object::set_scale(float s){
	this->s = s;
}

float Object::scale(){
	return s;
}

bool Object::load_obj(){
	this->has_texture = true;
	model = loader->get_model(path, s);
	
	return true;
}

void Object::get_mvp(float mvp[4][4]){
	float m1[4][4], m2[4][4];
	glGetFloatv(GL_MODELVIEW_MATRIX, m1[0]);
	glGetFloatv(GL_PROJECTION_MATRIX, m2[0]);
	mult_matrix(mvp, m1, m2);
}

void Object::get_matrix(float m[16]){
	get_transform()->getOpenGLMatrix(m);
}

btTransform* Object::get_transform(){
	btQuaternion rot(0, 0, 0, 1);
	trans = btTransform(rot, btVector3(x, y, z));
	return &trans;
}

