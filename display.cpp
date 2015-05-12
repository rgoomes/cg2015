
#include "display.hpp"

Object *colorCube;
Rigidbody *sphere;
Rigidbody *chair;
Object *dei;
Rigidbody *dei_collider;

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
	
	colorCube = new Object("objects/cube");
	colorCube->attach_loader(loader);
	colorCube->load_obj();
	colorCube->move(0, 20, 0);
	world->addObject(colorCube);


	sphere = new Rigidbody("objects/sphere", 5, btVector3(72, 30, 0));
	sphere->attach_loader(loader);
	sphere->set_scale(0.1);
	sphere->load_obj();
	world->addObject(sphere);

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
	
	/*for(int i=0; i<5; i++){
		chair = new Rigidbody("objects/chair", 10, btVector3(i*10, 60, -10+i*10));
		chair->attach_loader(loader);
		chair->set_scale(0.1);
		chair->load_obj(true);
		world->addObject(chair);
	}*/


	/*dei = new Object("objects/dei", loader);
	dei->set_scale(0.1);
	dei->load_obj(true);
	world->addObject(dei);*/
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

	
	world->update(elapsed);

	
}
