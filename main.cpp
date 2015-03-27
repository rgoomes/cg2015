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

GLFWwindow *window;

void monitor_resolution(int *w, int *h){
	int count;
	const GLFWvidmode* modes = glfwGetVideoModes(glfwGetPrimaryMonitor(), &count);

	int maxWidth = 0; int maxHeight = 0;
	for(int i = 0; i < count; i++){
		if(modes[i].width > maxWidth)
			maxWidth = modes[i].width;
		if(modes[i].height > maxHeight)
			maxHeight = modes[i].height;
	}

	*w = maxWidth;
	*h = maxHeight;
}

void opengl_init(int argc, char **argv){

	glfwInit();
	monitor_resolution(&SCREEN_WIDTH, &SCREEN_HEIGHT);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwSwapInterval(0);
	window = glfwCreateWindow(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, "cg2015", NULL, NULL);

	glfwMakeContextCurrent(window);
	glewInit();
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	GLuint p = load_shaders("SimpleVertexShader.glsl", "SimpleFragmentShader.glsl");
	//glUseProgram(p);

	glMatrixMode		(GL_PROJECTION);
	gluPerspective		(90, SCREEN_WIDTH/(float)SCREEN_HEIGHT, 1.0, 100.0);
	glMatrixMode		(GL_MODELVIEW);	
	gluLookAt			(2,2,4, 0,0,0, 0,1,0);

	load_objects();
	glEnable			(GL_DEPTH_TEST);
	add_lights();
}

int main(int argc, char **argv){
	opengl_init(argc, argv);

	do{
		display(window);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}while(glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS 
		&& glfwWindowShouldClose(window) == 0);

	return 0;
}
