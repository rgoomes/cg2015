
#include "main.hpp"

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
	monitor_resolution			(&SCREEN_WIDTH, &SCREEN_HEIGHT);
	glfwWindowHint				(GLFW_SAMPLES, 4);
	glfwWindowHint				(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint				(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwSwapInterval			(0);
	window = glfwCreateWindow	(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, "cg2015", NULL, NULL);

	glfwMakeContextCurrent		(window);
	glfwSetInputMode			(window, GLFW_STICKY_KEYS, GL_TRUE);

	glEnable					(GL_DEPTH_TEST);
	glDepthFunc					(GL_LESS);

	if (glewInit() != GLEW_OK) {
		fprintf(stdin, "Failed to initialize GLEW\n");
		exit(1);
	}
}

int main(int argc, char **argv){
	opengl_init(argc, argv);

	load_objects();
	
	do{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		display(window);

		glfwSwapBuffers(window);
		glfwPollEvents();

	} while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	glfwTerminate();

	return 0;
}
