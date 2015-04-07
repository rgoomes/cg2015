
#include "main.hpp"

using namespace std;

GLFWwindow *window;
int frame = 0;
double timebase = 0, tm = 0, last_tick=0;
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

btDiscreteDynamicsWorld* getDynamicWorld(){
	btBroadphaseInterface* broadphase = new btDbvtBroadphase();
	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

	return new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
}

btRigidBody* sphereRigidBody;
btDiscreteDynamicsWorld* dynamicsWorld;

void test_bullet(){
	dynamicsWorld = getDynamicWorld();

	dynamicsWorld->setGravity(btVector3(0, -10, 0));

	btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 1);
	btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)));
	btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));
	btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);
	dynamicsWorld->addRigidBody(groundRigidBody);


	btCollisionShape* sphereShape = new btSphereShape(6);
	btDefaultMotionState* sphereMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 40, 0)));
	btScalar mass = 1;
	btVector3 sphereInertia(0, 0, 0);
	sphereShape->calculateLocalInertia(mass, sphereInertia);
	btRigidBody::btRigidBodyConstructionInfo sphereRigidBodyCI(mass, sphereMotionState, sphereShape, sphereInertia); // mass, motionState, shape, inertia
	sphereRigidBody = new btRigidBody(sphereRigidBodyCI);
	dynamicsWorld->addRigidBody(sphereRigidBody);

}

void bullet_tick(float elapsed){
	dynamicsWorld->stepSimulation(elapsed, 60);

	btTransform trans;
	sphereRigidBody->getMotionState()->getWorldTransform(trans);

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

#ifdef __unix
	#if CONSOLE_FPS
		printf("\033[A\033[2Kfps: %lf\n", fps);
	#endif
#endif

	return elapsed;
}

int main(int argc, char **argv){
	opengl_init(argc, argv);

	last_tick = glfwGetTime();
	load_objects();
	test_bullet();

	do{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		btTransform trans;
		sphereRigidBody->getMotionState()->getWorldTransform(trans);

		float elapsed_time = frame_rate();
		bullet_tick(elapsed_time);
		
		//printf("%f\n", );

		display(window, elapsed_time);
		//printf("%f\n", elapsed_time);

		glfwSwapBuffers(window);
		glfwPollEvents();

	} while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	glfwTerminate();

	return 0;
}
