
#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include <btBulletDynamicsCommon.h>
#include "object.hpp"

typedef enum{MESH, CONVEXHULL, SPHERE, CUBE} ColliderType;

class Rigidbody : public Object	{

	public:
		btRigidBody* get_rigidbody();
		void load_obj(bool has_texture);
		Rigidbody(string path, btScalar mass, btVector3 pos);
		void move(float _x, float _y, float _z);
		void render();
		void render(float m[4][4]);
	private:
		btRigidBody* rigidbody;
		btCollisionShape* get_mesh_object();
	protected:
		btScalar mass;
};

#endif
