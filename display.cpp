#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/freeglut.h>

#include "display.hpp"
#include "object.hpp"
#include "misc.hpp"

Object cube("tests/cube.obj");
Object monkey("tests/suzanne.obj");

int frame = 0, timebase = 0, tm = 0;
double fps;

void frame_rate(){
	frame++;
	tm = glutGet(GLUT_ELAPSED_TIME);

	if(tm - timebase > 100){
		fps = frame*1000.0/(tm-timebase);
		timebase = tm;
		frame = 0;
#ifdef __unix
	#if CONSOLE_FPS
		printf("\033[A\033[2Kfps: %lf\n", fps);
	#endif
#endif
	}
}

void load_objects(){
	cube.load_obj(true);
	monkey.load_obj(true);
}

void add_lights(){
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	GLfloat	lightpos[4] = {5.0, 15.0, 10.0, 1.0}; 
	glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
}

void display(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.3,0.4,0.5, 1);

	glRotatef(0.1, 0, 1, 0);
	glutSolidTeapot(1.0);
	
	
	for(int i = 0; i < cube.out_vertices.size(); i += 3){
		glBegin(GL_TRIANGLES);
			glVertex3f(cube.out_vertices[i+0].x, cube.out_vertices[i+0].y, cube.out_vertices[i+0].z);
			glVertex3f(cube.out_vertices[i+1].x, cube.out_vertices[i+1].y, cube.out_vertices[i+1].z);
			glVertex3f(cube.out_vertices[i+2].x, cube.out_vertices[i+2].y, cube.out_vertices[i+2].z);
		glEnd();
	}
	

#if DISPLAY_FPS
	frame_rate();
#endif

	glutSwapBuffers();
}
