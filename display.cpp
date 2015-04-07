
#include "display.hpp"
#include "object.hpp"
#include "misc.hpp"

extern btRigidBody* sphereRigidBody;

Object chair("objects/chair");
Object colorCube("objects/cube");
Object dei("objects/dei");

float mvp[4][4];
float a=0;

void load_objects(){
	colorCube.set_scale(0.03);
	colorCube.load_obj(true);
	
	chair.set_scale(0.1);
	chair.move(20, 6, 0);
	chair.load_obj(true);

	dei.set_scale(0.1);
	dei.load_obj(true);

	add_lights();
}

void add_lights(){
	glDepthFunc(GL_LESS);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	GLfloat	lightpos[4] = {5.0, 15.0, 10.0, 1.0}; 
	glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
}

void get_mvp(float mvp[4][4]){
	float m1[4][4], m2[4][4];
	glGetFloatv(GL_PROJECTION_MATRIX, m2[0]);
	glGetFloatv(GL_MODELVIEW_MATRIX, m1[0]);
	mult_matrix(mvp, m1, m2);
}

void display(GLFWwindow* window, float elapsed){
	glClearColor(0.3,0.4,0.5, 1);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	int w, h;
	glfwGetWindowSize(window, &w, &h);

	gluPerspective(45.0f, w / (float)h, 0.1f, 5000.0f);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	a+=0.5*elapsed;
	gluLookAt(40*sin(a)+30,40,40*cos(a), 30,-1,0, 0, 1, 0);
	
	glTranslatef(20, 0, 0);
	get_mvp(mvp);

	//colorCube.render();
	btTransform trans;
	sphereRigidBody->getMotionState()->getWorldTransform(trans);
	chair.move(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ());
	chair.render(mvp);
	
	dei.render();
	
}
