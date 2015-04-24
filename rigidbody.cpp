
#include "rigidbody.hpp"

using namespace std;

Rigidbody::Rigidbody(string path, btScalar mass, btVector3 pos)
: Object(path){
	this->path = path;
	this->mass = mass;
	x = pos.getX();
	y = pos.getY();
	z = pos.getZ();
}

btCollisionShape* Rigidbody::get_mesh_object(){
	btTriangleMesh *mesh = new btTriangleMesh();
	int v;

	for (int i=0; i < (int)all_faces.size(); i++){
		v = all_faces[i].v_index[0]-1;
		btVector3 btVertex3(vertices[v].x, vertices[v].y, vertices[v].z);
		v = all_faces[i].v_index[1]-1;
		btVector3 btVertex2(vertices[v].x, vertices[v].y, vertices[v].z);
		v = all_faces[i].v_index[2]-1;
		btVector3 btVertex1(vertices[v].x, vertices[v].y, vertices[v].z);
		
		mesh->addTriangle(btVertex1, btVertex2, btVertex3);
	}

	btCollisionShape* shape  = new btConvexTriangleMeshShape(mesh);
	return shape;
}


void Rigidbody::load_obj(bool has_texture){
	Object::load_obj(has_texture);

	btCollisionShape* shape = get_mesh_object();
	btQuaternion rot(0, 0, 0, 1);
	//rot.setRotation(btVector3(0, 0, 1), 1.2);
	btDefaultMotionState* motion_state = new btDefaultMotionState(btTransform(rot, btVector3(x, y, z)));
	btVector3 inertia(0, 0, 0);
	shape->calculateLocalInertia(mass, inertia);
	btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(mass, motion_state, shape, inertia); // mass, motionState, shape, inertia
	rigidBodyCI.m_restitution = 1.0f;
	rigidBodyCI.m_friction = 0.2f;

	rigidbody = new btRigidBody(rigidBodyCI);
}

btRigidBody* Rigidbody::get_rigidbody(){
	return rigidbody;
}

void Rigidbody::get_matrix(float m[16]){
	btTransform trans;
	rigidbody->getMotionState()->getWorldTransform(trans);
	trans.getOpenGLMatrix(m);
}

void Rigidbody::move(float _x, float _y, float _z){
	x = _x;
	y = _y;
	z = _z;

	btTransform trans;
	rigidbody->getMotionState()->getWorldTransform(trans);
	trans.setOrigin(btVector3(x, y, z));
	rigidbody->getMotionState()->setWorldTransform(trans);
	
}

void Rigidbody::render_shadow(){
	float mt[16];
	glPushMatrix();
		get_matrix(mt);
		glMultMatrixf(mt);
		
		Object::render_shadow();
	glPopMatrix();
	
}

void Rigidbody::render(){
	/*btTransform trans;
	rigidbody->getMotionState()->getWorldTransform(trans);
	
	float mt[16];*/
	/*glPushMatrix();
		get_matrix(mt);
		glMultMatrixf(mt);
		
	glPopMatrix();*/
	
	Object::render();
	
}

void Rigidbody::render_texture(){
	
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

	for(int i=0; i<(int)groups.size(); i++){
		Group g = groups[i];
		if(g.size == 0)
			continue;

		glUseProgram(g.program_id);
		glUniformMatrix4fv(g.matrix_id, 1, GL_FALSE, &m[0][0]);
		glUniformMatrix4fv(g.depthbias_id, 1, GL_FALSE, &depthbias_mvp[0][0]);
		glUniformMatrix4fv(g.viewmatrix_id, 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(g.modelmatrix_id, 1, GL_FALSE, model);
		glUniform3f(g.lightdir_id, 0, 0, -1);
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, g.texture);
		glUniform1i(g.texture_id, 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, shadowmap);
		glUniform1i(g.shadowmap_id, 1);

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
		
	}


}
