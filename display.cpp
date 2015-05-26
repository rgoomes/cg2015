
#include "display.hpp"

Object *colorCube;
Rigidbody *sphere;
Rigidbody *chair;
Object *dei;
Rigidbody *dei_collider;
Rigidbody *box;
Object *sphere_aim;
Rigidbody *brick;

float mvp[4][4], a=0;
GLFWwindow* window;
World* world;
Loader* loader;
int ball_count = 0;

void set_environment(GLFWwindow* _window, World* _world, Loader* _loader){
	window = _window;
	world = _world;
	loader = _loader;
}

void load_objects(){
	int w, h;
	glfwGetWindowSize(window, &w, &h);

	for(int j=0; j<10; j++){
		for(int i=0; i<15; i++){
			brick = new Rigidbody("objects/brick", 30, btVector3(-5+2.42*j + (i%2)*1.25, 0.75+1.35*i , 25 * 0.03 ), CUBE, 1.211, 0.66, 0.5005);
			brick->attach_loader(loader);
			brick->set_scale(0.1);
			brick->load_obj();
			world->addObject(brick);
		}
	}

	box = new Rigidbody("objects/box", 0, btVector3(0, 2.0, 30), CONCAVE);
	box->attach_loader(loader);
	box->set_scale(0.2);
	box->load_obj();
	world->addObject(box);

	box = new Rigidbody("objects/box", 0, btVector3(203, 2.7, -56), CONCAVE);
	box->attach_loader(loader);
	box->set_scale(0.2);
	box->load_obj();
	box->rotate(btVector3(0, 1, 0), 45);
	box->rotate(btVector3(1, 0, 0), 5);
	//box->rotate(btVector3(0, 0, 1), 0);
	world->addObject(box);


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

	/*
	Rigidbody* cylinder = new Rigidbody("objects/cylinder", 5, btVector3(30,0,0));
	cylinder->attach_loader(loader);
	cylinder->set_scale(10);
	cylinder->load_obj();
	world->addObject(cylinder);
	*/

	btVector3 obs_pos = world->camera->get_obs_pos();
	sphere_aim = new Object("objects/sphere");
	sphere_aim->move(obs_pos.getX(), obs_pos.getY(), obs_pos.getZ());
	sphere_aim->attach_loader(loader);
	sphere_aim->set_scale(0.05);
	sphere_aim->load_obj();
	sphere_aim->is_static = true;
	world->addObject(sphere_aim);


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
	sphere = new Rigidbody("objects/sphere", 5, btVector3(obs_pos.getX() + n.getX()*10 , obs_pos.getY() + n.getY()*10, obs_pos.getZ() + n.getZ()*10 ), CONVEX, 0,0,0, 0.80);
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

int last_throw_state = GLFW_RELEASE;
bool throwing = false;

void request_throw(){	
	if(throwing){
		printf("\033[A\033[2KBall count: %d\n", ++ball_count);
		throw_ball();
	}
	
	int cur_state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) | glfwGetKey(window, GLFW_KEY_T);
	if(cur_state == GLFW_PRESS && cur_state != last_throw_state)
		throwing = true;
	else
		throwing = false;
	last_throw_state = cur_state;
}

void display(float elapsed){
	int w, h;
	glfwGetWindowSize(window, &w, &h);

	request_throw();
	world->update(elapsed);	
}
