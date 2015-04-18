
#include "rigidbody.hpp"
#include <btBulletDynamicsCommon.h>

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
	btTransform trans;
	rigidbody->getMotionState()->getWorldTransform(trans);
	
	float mt[16];
	glPushMatrix();
		get_matrix(mt);
		glMultMatrixf(mt);
		
		Object::render();
	glPopMatrix();
	
	
}
