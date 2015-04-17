
#include "main.hpp"

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

	*w = maxWidth;
	*h = maxHeight;
}

void opengl_init(){

	glfwInit();
	monitor_resolution			(&SCREEN_WIDTH, &SCREEN_HEIGHT);
	glfwWindowHint				(GLFW_SAMPLES, 1);
	glfwWindowHint				(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint				(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwSwapInterval			(0);
	main_window = glfwCreateWindow	(SCREEN_WIDTH/1.5, SCREEN_HEIGHT/1.5, "cg2015", NULL, NULL);

	glfwMakeContextCurrent		(main_window);
	glfwSetInputMode			(main_window, GLFW_STICKY_KEYS, GL_TRUE);

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

void bullet_init(){
	main_world = getDynamicWorld();

	main_world->setGravity(btVector3(0, -10, 0));

	btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 1);
	btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)));
	btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));
	btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);
	main_world->addRigidBody(groundRigidBody);
	
}

void bullet_tick(float elapsed){	
	main_world->stepSimulation(elapsed, 60);

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
	bullet_init();
	set_environment(main_window, main_world);
	
	main_world->stepSimulation(0.00001, 60);

	load_objects();
	load_skytextures();

	last_tick = glfwGetTime();
	
	do{

		float elapsed_time = frame_rate();
		bullet_tick(elapsed_time);

		display(elapsed_time);

		glfwSwapBuffers(main_window);
		glfwPollEvents();

	} while( glfwGetKey(main_window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(main_window) == 0 );

	glfwTerminate();

	return 0;
}
