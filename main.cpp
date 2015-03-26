#include <stdio.h>
#include <iostream>

#include <GL/glew.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>

#include "display.hpp"
#include "nvidia.hpp"
#include "main.hpp"
#include "shaders.hpp"

using namespace std;

GLFWwindow* window;

void GetMaxMonitorResolution(int* w, int* h)
{
    int count;
    const GLFWvidmode* modes = glfwGetVideoModes(glfwGetPrimaryMonitor(), &count);

    int maxWidth = 0;
    int maxHeight = 0;
    for (int i = 0; i < count; i++)
    {
        if (modes[i].width > maxWidth)
            maxWidth = modes[i].width;
        if (modes[i].height > maxHeight)
            maxHeight = modes[i].height;
    }

    *w = maxWidth;
    *h = maxHeight;
}

void opengl_init(int argc, char **argv){
	/*
	glutInit 			(&argc, argv);
	glutSetOption		(GLUT_MULTISAMPLE, 16);
	glutInitDisplayMode	(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glutInitWindowSize	(SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
	glutCreateWindow	("cg2015");
	*/

	/*GLFWmonitor screen;
	glfwGetVideoMode(&screen);
	SCREEN_HEIGHT = screen.Height;
	SCREEN_WIDTH = screen.Width;*/

	glfwInit();
	GetMaxMonitorResolution(&SCREEN_WIDTH, &SCREEN_HEIGHT);

	window = glfwCreateWindow( SCREEN_WIDTH/2, SCREEN_HEIGHT/2, "Tutorial 02 - Red triangle", NULL, NULL);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwSwapInterval(0);

	glfwMakeContextCurrent(window);
	glewInit();
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	GLuint p = load_shaders("SimpleVertexShader.glsl", "SimpleFragmentShader.glsl");
	//glUseProgram(p);

	glMatrixMode		(GL_PROJECTION);
	gluPerspective		(90, SCREEN_WIDTH/(float)SCREEN_HEIGHT, 1.0, 100.0);
	glMatrixMode		(GL_MODELVIEW);	
	gluLookAt			(2,2,4, 0,0,0, 0,1,0);

	//glutDisplayFunc		(display);
	//glutIdleFunc		(idle);

	load_objects();
	glEnable			(GL_DEPTH_TEST);
	add_lights();
	while(true)
		display(window);
}

int main(int argc, char **argv){
	opengl_init(argc, argv);
	//glutMainLoop();

	return 0;
}
