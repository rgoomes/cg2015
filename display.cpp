
#include "display.hpp"

Object *colorCube;
Rigidbody *sphere;
Rigidbody *chair;
Object *dei;
Rigidbody *dei_collider;
Rigidbody *box;
Object *sphere_aim;

float mvp[4][4], a=0;
GLFWwindow* window;
World* world;
Loader* loader;

void set_environment(GLFWwindow* _window, World* _world, Loader* _loader){
	window = _window;
	world = _world;
	loader = _loader;
}

void load_objects(){
	int w, h;
	glfwGetWindowSize(window, &w, &h);

	box = new Rigidbody("objects/box", 0, btVector3(202, 2.6, -58), CONCAVE);
	box->attach_loader(loader);
	box->set_scale(0.15);
	box->load_obj();
	world->addObject(box);

	for(int i=0; i<30; i++){
		sphere = new Rigidbody("objects/sphere", 5, btVector3(170, 50+5*i, 75));
		sphere->attach_loader(loader);
		sphere->set_scale(0.05);
		sphere->load_obj();
		world->addObject(sphere);
	}

	dei = new Object("objects/dei");
	dei->attach_loader(loader);
	dei->set_scale(0.1);
	dei->load_obj();
	world->addObject(dei);

	dei_collider = new Rigidbody("objects/dei_collider", 0, btVector3(0, 0, 0), CONCAVE);
	dei_collider->attach_loader(loader);
	dei_collider->set_scale(0.1);
	dei_collider->load_obj();
	world->addCollider(dei_collider);

	Object* cylinder = new Object("objects/cylinder");
	cylinder->set_scale(10);
	cylinder->move(30, 0, 0);
	cylinder->attach_loader(loader);
	cylinder->load_obj();
	world->addObject(cylinder);


	btVector3 obs_pos = world->camera->get_obs_pos();
	sphere_aim = new Object("objects/sphere");
	sphere_aim->move(obs_pos.getX(), obs_pos.getY(), obs_pos.getZ());
	sphere_aim->attach_loader(loader);
	sphere_aim->set_scale(0.05);
	sphere_aim->load_obj();
	world->addObject(sphere_aim);

	sphere_aim->is_static = true;
}

void load_textures(){
	load_skybox();

}

void add_lights(){
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void throw_ball(){
	btVector3 obs_pos = world->camera->get_obs_pos();
	btVector3 n = world->camera->get_direction();

	n.normalize();
	sphere = new Rigidbody("objects/sphere", 5, btVector3(obs_pos.getX() + n.getX()*10 , obs_pos.getY() + n.getY()*10, obs_pos.getZ() + n.getZ()*10 ) );
	sphere->attach_loader(loader);
	sphere->set_scale(0.05);
	sphere->load_obj();
	world->addObject(sphere);

	btRigidBody* r = sphere->get_rigidbody();
	r->setLinearVelocity(world->camera->get_direction() * 100);

}

void get_mvp(float mvp[4][4]){
	float m1[4][4], m2[4][4], t[4][4];
	glGetFloatv(GL_MODELVIEW_MATRIX, m1[0]);
	glGetFloatv(GL_PROJECTION_MATRIX, m2[0]);
	mult_matrix(mvp, m1, m2);
}

GLfloat light_position[] = { 1.0, 15.0, -30.0, 1.0 };

void display(float elapsed){
	int w, h;
	glfwGetWindowSize(window, &w, &h);
	

	if(glfwGetKey(window, GLFW_KEY_T))
		throw_ball();
	
	world->update(elapsed);

	
}
