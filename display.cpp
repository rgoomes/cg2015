
#include "display.hpp"

#define PI 3.14159

Rigidbody colorCube("objects/cube", 1, btVector3(10, 30, 0));
Object dei("objects/dei");
Rigidbody chair("objects/chair", 10, btVector3(20, 30, 20));

float mvp[4][4];
float a=0;
GLFWwindow* window;
btDynamicsWorld* world;

double ray=5.0, ang=0.5*PI+90, inc=100;
double obs_ini[] = {60,18,-10};
double obs_fin[] = {obs_ini[0]-ray*cos(ang), obs_ini[1], obs_ini[2]-ray*sin(ang)};

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

	add_lights();
	
}

void add_lights(){
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
}

void get_mvp(float mvp[4][4]){
	float m1[4][4], m2[4][4], t[4][4];
	glGetFloatv(GL_MODELVIEW_MATRIX, m1[0]);
	glGetFloatv(GL_PROJECTION_MATRIX, m2[0]);
	mult_matrix(mvp, m1, m2);
}

void camera_view(float elapsed){
	if(glfwGetKey(window, GLFW_KEY_UP)   || glfwGetKey(window, GLFW_KEY_W)){
		obs_ini[0] = obs_ini[0] + elapsed*inc*cos(ang);
		obs_ini[2] = obs_ini[2] - elapsed*inc*sin(ang);
	}
	if(glfwGetKey(window, GLFW_KEY_DOWN)  || glfwGetKey(window, GLFW_KEY_S)){
		obs_ini[0] = obs_ini[0] - elapsed*inc*cos(ang);
		obs_ini[2] = obs_ini[2] + elapsed*inc*sin(ang);
	}
	if(glfwGetKey(window, GLFW_KEY_LEFT)  || glfwGetKey(window, GLFW_KEY_A))
		ang = (ang + elapsed * 1.0);
	if(glfwGetKey(window, GLFW_KEY_RIGHT) || glfwGetKey(window, GLFW_KEY_D))
		ang = (ang - elapsed * 1.0);

	obs_fin[0] = obs_ini[0] + ray * cos(ang);
	obs_fin[2] = obs_ini[2] - ray * sin(ang);

	gluLookAt(obs_ini[0],obs_ini[1],obs_ini[2], obs_fin[0],obs_fin[1],obs_fin[2], 0,1,0);
}

void display(float elapsed){
	int w, h;
	glfwGetWindowSize(window, &w, &h);

	glClearColor(0.3,0.4,0.5, 1);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, w / (float)h, 0.1f, 5000.0f);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	// a+=0.5*elapsed;
	// gluLookAt(80*sin(a)-30,40,80*cos(a), 30,-1,0, 0, 1, 0);
	camera_view(elapsed);

	GLfloat	lightpos[4] = {10.0, 20.0, 0.0, 1.0}; 
	glLightfv(GL_LIGHT0, GL_POSITION, lightpos);

	chair.render();
	colorCube.render();

	glTranslatef(20, 0, 0);
	dei.render_ntexture();
	
}
