#include <GL/glew.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>

#include "display.hpp"
#include "object.hpp"
#include "misc.hpp"

Object chair("objects/chair");
Object colorCube("objects/cube");

int frame = 0;
double timebase = 0, tm = 0;
double fps;

float mvp[4][4];
float a=0;

void frame_rate(){
	frame++;
	tm = glfwGetTime();

	if(tm - timebase > 0.1){
		fps = frame/(tm-timebase);
		timebase = tm;
		frame = 0;
	}

#ifdef __unix
	#if CONSOLE_FPS
		printf("\033[A\033[2Kfps: %lf\n", fps);
	#endif
#endif

}

void load_objects(){
	colorCube.set_scale(0.03);
	colorCube.load_obj(true);
	
	chair.set_scale(0.03);
	chair.load_obj(true);
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

void display(GLFWwindow* window){
	glClearColor(0.3,0.4,0.5, 1);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	int w, h;
	glfwGetWindowSize(window, &w, &h);

	gluPerspective(45.0f, w / (float)h, 0.1f, 100.0f);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(3*sin(a),3*sin(a),3*cos(a), 0,-1,0, 0, 1, 0);
	a+=0.01;
	
	get_mvp(mvp);

	colorCube.render(mvp);
	chair.render(mvp);
	
#if DISPLAY_FPS
	frame_rate();
#endif

}
