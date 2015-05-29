
#include "world.hpp"
#include "object.hpp"
#include "rigidbody.hpp"

btDynamicsWorld* World::getDynamicWorld(){
	btBroadphaseInterface* broadphase = new btDbvtBroadphase();
	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

	return new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
}

void World::init(){
	physicsWorld = getDynamicWorld();
	physicsWorld->setGravity(btVector3(0, -20, 0));
}

bool paused = false;
int last_pause_state = GLFW_RELEASE;

void World::tick(float elapsed){
	if(!paused)
		physicsWorld->stepSimulation(elapsed, 60);
	else
		this->timer->stop();

	int cur_state = glfwGetKey(window, GLFW_KEY_P);
	if(cur_state == GLFW_PRESS && cur_state != last_pause_state)
		paused = !paused;
	last_pause_state = cur_state;
}

void World::addCollider(Object* obj){
	physicsWorld->addRigidBody(((Rigidbody*)obj)->get_rigidbody());
	
}

void World::addObject(Object* obj){
	obj->set_shadowmap(renderedTexture);
	objects.push_back(obj);
	obj->attach_world(this);
	if(obj->type() == "rigidbody")
		physicsWorld->addRigidBody(((Rigidbody*)obj)->get_rigidbody());
	
}

GLuint World::get_render_buffer(){
	int w, h;
	glfwGetWindowSize(window, &w, &h);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
	glGenFramebuffers(1, &FramebufferName);
	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);

	// Depth texture. Slower than a depth buffer, but you can sample it later in your shader
	GLuint depthTexture;
	glGenTextures(1, &depthTexture);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0,GL_DEPTH_COMPONENT16, SHADOW_RES, SHADOW_RES, 0,GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);

	// No color output in the bound framebuffer, only depth.
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	// Always check that our framebuffer is ok
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		printf("Error creating depth render buffer.\n");

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return depthTexture;
}

void World::update(float elapsed){
	int i;

	tick(elapsed);

	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
	glViewport(0, 0, SHADOW_RES, SHADOW_RES);

	glDisable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(-200, 310, -150, 270, -250, 150); // directional light

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(-0.5, 0.8, 0.5, 0, 0, 0, 0, 1, 0);

	for(i=0; i<(int)objects.size();  i++){
		objects[i]->render_shadow();
	}

	// SCREEN RENDERING

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, window_width, window_height);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, window_width / (float)window_height, 0.1f, 10000.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	camera->update(window, elapsed);
	draw_skybox(1000);

	/*GLfloat light_position[] = { 1.0, 15.0, -30.0, 1.0 };

	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);*/

	for(i=0; i<(int)objects.size();  i++){
		if(objects[i]->is_static){
			btVector3 obs_pos = camera->get_obs_pos();
			btVector3 n = camera->get_direction();
			btVector3 r = camera->get_right();
			r.normalize();
			n.normalize();

			n -= r / 5;
			objects[i]->move(obs_pos.getX() + n.getX()*10 , obs_pos.getY() + n.getY()*10, obs_pos.getZ() + n.getZ()*10);
		}

		objects[i]->render_texture();
	}
	for(i=0; i<(int)objects.size();  i++){
		objects[i]->render_glass();
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}


World::World(GLFWwindow* window){
	this->window = window;
	glfwGetWindowSize(window, &window_width, &window_height);

	physicsWorld = getDynamicWorld();
	init();

	renderedTexture = get_render_buffer();

	camera = new Camera(window_width, window_height);

	timer = new Timer();
}
