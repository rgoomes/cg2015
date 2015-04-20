
#include "display.hpp"


Rigidbody *colorCube;
Rigidbody *sphere;
Rigidbody *chair;
Object *dei;

float mvp[4][4], a=0;
GLFWwindow* window;
World* world;

void set_environment(GLFWwindow* _window, World* _world){
	window = _window;
	world = _world;
}


void load_objects(){
	int w, h;
	glfwGetWindowSize(window, &w, &h);
	glfwSetCursorPos(window, w/2, h/2);
	
	colorCube = new Rigidbody("objects/cube", 5, btVector3(10, 20, 40));
	sphere = new Rigidbody("objects/sphere", 5, btVector3(20, 20, 20));
	chair = new Rigidbody("objects/chair", 10, btVector3(20, 30, 10));
	dei = new Object("objects/dei");

	colorCube->set_scale(1);
	colorCube->load_obj(true);
	world->addObject(colorCube);
	
	chair->set_scale(0.1);
	chair->load_obj(true);
	world->addObject(chair);

	sphere->set_scale(0.1);
	sphere->load_obj(true);
	world->addObject(sphere);

	dei->set_scale(0.1);
	dei->load_obj(true);
}

void load_textures(){
	load_skybox();

}

void add_lights(){
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);

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

	dei->render_ntexture();
	
}
