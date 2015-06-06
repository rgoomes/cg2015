
#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include <btBulletDynamicsCommon.h>
#include "object.hpp"

typedef enum{CONVEX, CONCAVE, CONVEXHULL, SPHERE, CUBE} ColliderType;

class Object;

class Rigidbody : public Object{

	public:
		btRigidBody* get_rigidbody();
		void load_obj();
		Rigidbody(string path, btScalar mass, btVector3 pos, ColliderType collider=CONVEX, double w=0, double h=0, double d=0, double restitution=0.50f, double friction=0.95f);
		void move(float _x, float _y, float _z);
		void get_matrix(float m[16]);
		virtual string type(){return "rigidbody";}
		void rotate(btVector3 up, float degree);
		btVector3 get_position();
		bool contains(Rigidbody* r, btScalar rd);
		void render_texture(){Object::render_texture();};
	private:
		btRigidBody* rigidbody;
		btCollisionShape* shape;
		btCollisionShape* get_mesh_object(double w=0, double h=0, double d=0);
		btTransform* get_transform();
		btTransform trans;
		ColliderType collider_type = CONVEX;
		double w, h, d;
		double restitution;
		double friction;
	protected:
		btScalar mass;
};

#endif
