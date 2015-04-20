
#include "display.hpp"

#define PI 3.141592653

/*Rigidbody colorCube("objects/cube", 1, btVector3(10, 30, 0));

*/
Rigidbody colorCube("objects/cube", 5, btVector3(20, 20, 40));
Rigidbody sphere("objects/sphere", 5, btVector3(20, 20, 20));
Rigidbody chair("objects/chair", 10, btVector3(20, 30, 10));
//Object chair("objects/chair");
//Object sphere("objects/sphere");
Object dei("objects/dei");

float mvp[4][4], a=0;
GLFWwindow* window;
btDynamicsWorld* world;

double horizontal_ang = PI/8, vertical_ang = PI/4;
double mouse_speed = 0.002f, speed = 90.0f, xpos, ypos;
btVector3 obs_pos(60, 18, -10);

void set_environment(GLFWwindow* _window, btDynamicsWorld* _world){
	window = _window;
	world = _world;
}

GLuint renderedTexture;
GLuint FramebufferName = 0, depthTexture;

void load_objects(){
	int w, h;
	glfwGetWindowSize(window, &w, &h);
	glfwSetCursorPos(window, w/2, h/2);
	
	get_render_buffer();
		
	colorCube.set_scale(1);
	colorCube.load_obj(true);
	colorCube.set_shadowmap(depthTexture);
	world->addRigidBody(colorCube.get_rigidbody());
	
	chair.set_scale(0.1);
	chair.load_obj(true);
	chair.set_shadowmap(depthTexture);
	world->addRigidBody(chair.get_rigidbody());

	dei.set_scale(0.1);
	dei.set_shadowmap(depthTexture);
	dei.load_obj(true);

	sphere.set_scale(0.1);
	sphere.load_obj(true);
	sphere.set_shadowmap(depthTexture);
	world->addRigidBody(sphere.get_rigidbody());

}

void load_textures(){
	load_skybox();

}

void add_lights(){
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);

}

void get_mvp(float mvp[4][4]){
	float m1[4][4], m2[4][4], t[4][4];
	glGetFloatv(GL_MODELVIEW_MATRIX, m1[0]);
	glGetFloatv(GL_PROJECTION_MATRIX, m2[0]);
	mult_matrix(mvp, m1, m2);
}

void camera_view(float elapsed, int w, int h){
	glfwGetCursorPos(window, &xpos, &ypos);
	glfwSetCursorPos(window, w/2, h/2);

	horizontal_ang += mouse_speed * double(w/2 - xpos);
	vertical_ang   -= mouse_speed * double(h/2 - ypos);

	btVector3 dir(cos(vertical_ang)*sin(horizontal_ang), sin(vertical_ang), cos(vertical_ang)*cos(horizontal_ang));
	btVector3 right(sin(horizontal_ang - PI/2.0f), 0, cos(horizontal_ang - PI/2.0f));

	if(glfwGetKey(window, GLFW_KEY_UP)    || glfwGetKey(window, GLFW_KEY_W))
		obs_pos += dir * elapsed * speed;
	if(glfwGetKey(window, GLFW_KEY_DOWN)  || glfwGetKey(window, GLFW_KEY_S))
		obs_pos -= dir * elapsed * speed;
	if(glfwGetKey(window, GLFW_KEY_RIGHT) || glfwGetKey(window, GLFW_KEY_D))
		obs_pos -= right * elapsed * speed;
	if(glfwGetKey(window, GLFW_KEY_LEFT)  || glfwGetKey(window, GLFW_KEY_A))
		obs_pos += right * elapsed * speed;

	double obs_x = obs_pos.getX();
	double obs_y = obs_pos.getY();
	double obs_z = obs_pos.getZ();
	
	if(obs_x > WORLD_MAX_X)
		obs_pos.setX(WORLD_MAX_X);
	else if(obs_x < WORLD_MIN_X)
		obs_pos.setX(WORLD_MIN_X);
	//if(obs_y > WORLD_MAX_Y)
	//t	obs_pos.setY(WORLD_MAX_Y);
	else if(obs_y < WORLD_MIN_Y)
		obs_pos.setY(WORLD_MIN_Y);
	if(obs_z > WORLD_MAX_Z)
		obs_pos.setZ(WORLD_MAX_Z);
	else if(obs_z < WORLD_MIN_Z)
		obs_pos.setZ(WORLD_MIN_Z);

	btVector3 tmp = obs_pos+dir;
	btVector3 up = -dir.cross(right);

	gluLookAt(obs_pos.getX(),obs_pos.getY(),obs_pos.getZ(), 
			      tmp.getX(),    tmp.getY(),    tmp.getZ(), 
			      -up.getX(),    -up.getY(),    -up.getZ()
	);
}


GLfloat light_position[] = { 1.0, 15.0, -30.0, 1.0 };

void get_render_buffer(){
	int w, h;
	glfwGetWindowSize(window, &w, &h);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
	glGenFramebuffers(1, &FramebufferName);
	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);

	// Depth texture. Slower than a depth buffer, but you can sample it later in your shader
	glGenTextures(1, &depthTexture);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0,GL_DEPTH_COMPONENT16, w, h, 0,GL_DEPTH_COMPONENT, GL_FLOAT, 0);
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

}

void display(float elapsed){
	int w, h;
	glfwGetWindowSize(window, &w, &h);

	// RENDER BUFFER RENDERING

	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
	glViewport(0, 0, w, h);
	
	glDisable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-100, 100, -100, 100, -100, 100); // directional light

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(0, 0, -1, 0, 0, 0, 0, 1, 0);

	//glEnable(GL_LIGHT0);
	//glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	colorCube.render_shadow();
	sphere.render_shadow();
	chair.render_shadow();
	//dei.render_shadow();

	// SCREEN RENDERING

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, w / (float)h, 0.1f, 1000.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	camera_view(elapsed, w, h);
	draw_skybox(500);

	GLfloat light_position[] = { 1.0, 15.0, -30.0, 1.0 };

	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	//glEnable(GL_LIGHT0);
	//glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	colorCube.render();
	sphere.render();
	chair.render();

	dei.render_ntexture();
	//glTranslatef(20, 0, 0);

}
