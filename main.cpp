
#include "main.hpp"
#include "display.hpp"

#define SCREEN_SIZE_CUT 1.2f

using namespace std;

int frame = 0;
double timebase = 0, tm = 0, last_tick;
double fps;

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

	*w = maxWidth  / SCREEN_SIZE_CUT;
	*h = maxHeight / SCREEN_SIZE_CUT;
}

void opengl_init(){
	glfwInit();
	monitor_resolution			(&SCREEN_WIDTH, &SCREEN_HEIGHT);
	glfwWindowHint				(GLFW_SAMPLES, 2);
	glfwWindowHint				(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint				(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwSwapInterval			(0);
	main_window = glfwCreateWindow	(SCREEN_WIDTH, SCREEN_HEIGHT, "cg2015", NULL, NULL);
 	glfwSetInputMode			(main_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
 	
	glfwMakeContextCurrent		(main_window);
	glfwSetCursorPos			(main_window, SCREEN_WIDTH/2, SCREEN_HEIGHT/2);

	glEnable					(GL_DEPTH_TEST);
	glDepthFunc					(GL_LESS);

	if (glewInit() != GLEW_OK) {
		fprintf(stdin, "Failed to initialize GLEW\n");
		exit(1);
	}
}

btDynamicsWorld* getDynamicWorld(){
	btBroadphaseInterface* broadphase = new btDbvtBroadphase();
	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

	return new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
}

float frame_rate(){
	frame++;
	tm = glfwGetTime();
	float elapsed = tm - last_tick;
	last_tick = tm;

	if(tm - timebase > 0.1){
		fps = frame/(tm-timebase);
		timebase = tm;
		frame = 0;
	}

#if CONSOLE_FPS
	printf("\033[A\033[2Kfps: %lf\n", fps);
#endif

	return elapsed;
}

int main(int argc, char **argv){
	opengl_init();
	srand(time(NULL));
	
	main_world = new World(main_window);
	main_loader = new Loader();
	set_environment(main_window, main_world, main_loader);

	load_objects();
	load_textures();
	add_lights();
	init_sizes(SCREEN_WIDTH, SCREEN_HEIGHT);
	init_freetype();

	last_tick = glfwGetTime();
	
	do{

		float elapsed_time = frame_rate();

		glColor4f(1, 1, 1, 1);
		display(elapsed_time);

		glfwSwapBuffers(main_window);
		glfwPollEvents();

	} while( glfwGetKey(main_window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(main_window) == 0 );

	glfwTerminate();

	return 0;
}
