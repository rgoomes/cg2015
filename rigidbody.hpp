
#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include <btBulletDynamicsCommon.h>
#include "object.hpp"

typedef enum{MESH, CONVEXHULL, SPHERE, CUBE} ColliderType;

class Object;

class Rigidbody : public Object{

	public:
		btRigidBody* get_rigidbody();
		void load_obj();
		Rigidbody(string path, btScalar mass, btVector3 pos);
		void move(float _x, float _y, float _z);
		void get_matrix(float m[16]);
		void render_texture();
		virtual string type(){return "rigidbody";}
	private:
		btRigidBody* rigidbody;
		btCollisionShape* get_mesh_object();
		btTransform* get_transform();
		btTransform trans;
	protected:
		btScalar mass;
};

#endif
