
#include "display.hpp"

//Object chair("objects/chair");

Rigidbody colorCube("objects/cube", 1, btVector3(10, 30, 0));
Object dei("objects/dei");
Rigidbody chair("objects/chair", 10, btVector3(10, 30, 0));

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
	//world->addRigidBody(colorCube.get_rigidbody());
	
	chair.set_scale(0.1);
	chair.load_obj(true);
	world->addRigidBody(chair.get_rigidbody());

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

void get_mvp(float mvp[4][4], float rot[16]){
	float m1[4][4], m2[4][4], t[4][4];
	glGetFloatv(GL_PROJECTION_MATRIX, m2[0]);
	glGetFloatv(GL_MODELVIEW_MATRIX, m1[0]);
	//mult_matrix(t, rot, m1);
	mult_matrix(mvp, m1, m2);
}

void display(float elapsed){
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

	float mt[16];
	

	glPushMatrix();
		chair.get_matrix(mt);
		
		glMultMatrixf(mt);

		get_mvp(mvp, mt);
		
		chair.render();
	glPopMatrix();
	
		
	glTranslatef(20, 0, 0);	
	dei.render();
	
}
