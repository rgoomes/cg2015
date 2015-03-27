#include <GL/glew.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>

#include "display.hpp"
#include "object.hpp"
#include "misc.hpp"

Object chair("tests/chair.obj");

int frame = 0;
double timebase = 0, tm = 0;
double fps;

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
	chair.set_scale(0.1);
	chair.load_obj(true);
}

void add_lights(){
	glDepthFunc(GL_LESS);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	GLfloat	lightpos[4] = {5.0, 15.0, 10.0, 1.0}; 
	glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
}

void display(GLFWwindow* window){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.3,0.4,0.5, 1);

	glRotatef(0.1, 0, 1, 0);
	chair.render();
	
#if DISPLAY_FPS
	frame_rate();
#endif

}
