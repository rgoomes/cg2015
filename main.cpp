#include <stdio.h>
#include <iostream>

#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/freeglut.h>

#include "display.hpp"
#include "nvidia.hpp"
#include "main.hpp"

using namespace std;

void idle(void){ glutPostRedisplay(); }

void opengl_init(int argc, char **argv){
	glutInit 			(&argc, argv);
	glutInitDisplayMode	(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize	(SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
	glutCreateWindow	("cg2015");

	glMatrixMode		(GL_PROJECTION);
	gluPerspective		(90, SCREEN_WIDTH/(float)SCREEN_HEIGHT, 1.0, 100.0);
	glMatrixMode		(GL_MODELVIEW);	
	gluLookAt			(2,2,4, 0,0,0, 0,1,0);

	glutDisplayFunc		(display);
	glutIdleFunc		(idle);

	load_objects();
	add_lights();
}

int main(int argc, char **argv){
	opengl_init(argc, argv);
	glutMainLoop();

	return 0;
}
