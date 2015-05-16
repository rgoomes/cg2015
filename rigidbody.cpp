
#include "rigidbody.hpp"

using namespace std;

Rigidbody::Rigidbody(string path, btScalar mass, btVector3 pos, ColliderType collider)
: Object(path){
	this->path = path;
	this->mass = mass;
	this->collider_type = collider;
	x = pos.getX();
	y = pos.getY();
	z = pos.getZ();
}

btCollisionShape* Rigidbody::get_mesh_object(){
	btCollisionShape* shape;

	if(collider_type == CONCAVE){
		btTriangleMesh *mesh = new btTriangleMesh();
		int v;

		for (int i=0; i < (int)model->all_faces.size(); i++){
			v = model->all_faces[i].v_index[0]-1;
			btVector3 btVertex3(model->vertices[v].x, model->vertices[v].y, model->vertices[v].z);
			v = model->all_faces[i].v_index[1]-1;
			btVector3 btVertex2(model->vertices[v].x, model->vertices[v].y, model->vertices[v].z);
			v = model->all_faces[i].v_index[2]-1;
			btVector3 btVertex1(model->vertices[v].x, model->vertices[v].y, model->vertices[v].z);
			
			mesh->addTriangle(btVertex1, btVertex2, btVertex3);
		}
		shape = new btBvhTriangleMeshShape(mesh, true);
		


		
	}else{
		btConvexHullShape* chs = new btConvexHullShape();

		int v;
		for (int i=0; i < (int)model->all_faces.size(); i++){
			v = model->all_faces[i].v_index[0]-1;
			btVector3 btVertex3(model->vertices[v].x, model->vertices[v].y, model->vertices[v].z);
			chs->addPoint(btVertex3);
			v = model->all_faces[i].v_index[1]-1;
			btVector3 btVertex2(model->vertices[v].x, model->vertices[v].y, model->vertices[v].z);
			chs->addPoint(btVertex2);
			v = model->all_faces[i].v_index[2]-1;
			btVector3 btVertex1(model->vertices[v].x, model->vertices[v].y, model->vertices[v].z);
			chs->addPoint(btVertex1);
			
		}
		shape = chs;

	}

	return shape;
}


void Rigidbody::load_obj(){
	has_texture = true;
	Object::load_obj();

	btCollisionShape* shape = get_mesh_object();
	btQuaternion rot(0, 0, 0, 1);
	//rot.setRotation(btVector3(0, 0, 1), 1.2);
	btDefaultMotionState* motion_state = new btDefaultMotionState(btTransform(rot, btVector3(x, y, z)));
	btVector3 inertia(0, 0, 0);
	shape->calculateLocalInertia(mass, inertia);
	btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(mass, motion_state, shape, inertia); // mass, motionState, shape, inertia
	rigidBodyCI.m_restitution = 0.95f;
	rigidBodyCI.m_friction = 0.2f;
	rigidBodyCI.m_angularDamping = 0.1f;

	rigidbody = new btRigidBody(rigidBodyCI);
}

btRigidBody* Rigidbody::get_rigidbody(){
	return rigidbody;
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


btTransform* Rigidbody::get_transform(){
	rigidbody->getMotionState()->getWorldTransform(trans);
	return &trans;
}
