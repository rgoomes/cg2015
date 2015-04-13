
#include "display.hpp"

Rigidbody colorCube("objects/cube", 1, btVector3(10, 30, 0));
Object dei("objects/dei");
Rigidbody chair("objects/chair", 10, btVector3(20, 30, 20));
Rigidbody sphere("objects/sphere", 5, btVector3(20, 20, 20));

float mvp[4][4];
float a=0;
GLFWwindow* window;
btDynamicsWorld* world;

void set_environment(GLFWwindow* _window, btDynamicsWorld* _world){
	window = _window;
	world = _world;
}

void load_objects(){
	colorCube.set_scale(1);
	colorCube.load_obj(true);
	world->addRigidBody(colorCube.get_rigidbody());
	
	chair.set_scale(0.1);
	chair.load_obj(true);
	world->addRigidBody(chair.get_rigidbody());

	dei.set_scale(0.1);
	dei.load_obj(true);

	sphere.set_scale(0.1);
	sphere.load_obj(true);
	world->addRigidBody(sphere.get_rigidbody());

	add_lights();
	
}

void add_lights(){
	glShadeModel (GL_SMOOTH);
   
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
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

	glClearColor(0.3,0.4,0.5, 1);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, w / (float)h, 0.1f, 5000.0f);

	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	a = 45;
	gluLookAt(80*sin(a)-30,40,80*cos(a), 30,-1,0, 0, 1, 0);

	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	chair.render();
	colorCube.render();
	sphere.render();

	glTranslatef(20, 0, 0);
	dei.render_ntexture();
	
}

