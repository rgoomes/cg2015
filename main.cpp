#include <stdio.h>
#include <iostream>

#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/freeglut.h>

#include "object.hpp"
#include "nvidia.hpp"
#include "main.hpp"

using namespace std;

int main(int argc, char **argv){
	opengl_init(argc, argv);
	glutMainLoop();

	return 0;
}
