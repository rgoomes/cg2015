
#include "display.hpp"

#define PI 3.141592653

Rigidbody colorCube("objects/cube", 1, btVector3(10, 30, 0));
Object dei("objects/dei");
Rigidbody chair("objects/chair", 10, btVector3(20, 30, 10));
Rigidbody sphere("objects/sphere", 5, btVector3(20, 20, 20));

float mvp[4][4];
float a=0;
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

void load_objects(){
	colorCube.set_scale(1);
	colorCube.load_obj(true);
	world->addRigidBody(colorCube.get_rigidbody());
	
	chair.set_scale(0.1);
	chair.load_obj(true);
	//world->addRigidBody(chair.get_rigidbody());

	dei.set_scale(0.1);
	dei.load_obj(true);

	sphere.set_scale(0.1);
	sphere.load_obj(true);
	world->addRigidBody(sphere.get_rigidbody());

	add_lights();


	int w, h;
	glfwGetWindowSize(window, &w, &h);

	glfwSetCursorPos(window, w/2, h/2);
	renderedTexture = get_render_buffer();
}

void add_lights(){
	glShadeModel (GL_SMOOTH);
   
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);

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

	printf("%lf %lf\n", w/2-xpos, h/2-ypos);
	horizontal_ang += mouse_speed * double(w/2 - xpos);
	vertical_ang   += mouse_speed * double(h/2 - ypos);

	btVector3 dir(cos(vertical_ang)*sin(horizontal_ang), sin(vertical_ang), cos(vertical_ang)*cos(horizontal_ang));
	btVector3 right(sin(horizontal_ang - PI/2.0f), 0, cos(horizontal_ang - PI/2.0f));

	if(glfwGetKey(window, GLFW_KEY_UP)    || glfwGetKey(window, GLFW_KEY_W))
		obs_pos += dir * elapsed * speed;
	if(glfwGetKey(window, GLFW_KEY_DOWN)  || glfwGetKey(window, GLFW_KEY_S))
		obs_pos -= dir * elapsed * speed;
	if(glfwGetKey(window, GLFW_KEY_RIGHT) || glfwGetKey(window, GLFW_KEY_D))
		obs_pos += right * elapsed * speed;
	if(glfwGetKey(window, GLFW_KEY_LEFT)  || glfwGetKey(window, GLFW_KEY_A))
		obs_pos -= right * elapsed * speed;
	
	btVector3 tmp = obs_pos+dir;
	btVector3 up = -dir.cross(right);
	gluLookAt(obs_pos.getX(),obs_pos.getY(),obs_pos.getZ(), 
			  tmp.getX(),tmp.getY(),tmp.getZ(), 
			  up.getX(), up.getY(), up.getZ()
	);
}

GLfloat light_position[] = { 1.0, 15.0, -30.0, 1.0 };

GLuint FramebufferName = 0;

int get_render_buffer(){
	int w, h;
	glfwGetWindowSize(window, &w, &h);
	printf("\n\n asdasdas %d %d\n\n", w, h);


	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
	glGenFramebuffers(1, &FramebufferName);
	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);

	// The texture we're going to render to
	GLuint renderedTexture;
	glGenTextures(1, &renderedTexture);
	 
	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, renderedTexture);
	 
	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA8, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Give an empty image to OpenGL ( the last "0" )
	
	if ( !GLEW_ARB_framebuffer_object ){ // OpenGL 2.1 doesn't require this, 3.1+ does
		printf("Your GPU does not provide framebuffer objects. Use a texture instead.");
		return -1;
	}

	// The depth buffer
	GLuint depthrenderbuffer;
	glGenRenderbuffers(1, &depthrenderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, w, h);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderedTexture, 0);

	// Set "renderedTexture" as our colour attachement #0
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderedTexture, 0);
	 
	// Set the list of draw buffers.
	GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
	glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

	// Always check that our framebuffer is ok
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		printf("error\n");

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return renderedTexture;
}

void display(float elapsed){
	int w, h;
	glfwGetWindowSize(window, &w, &h);

	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_GREATER);

	glViewport(0, 0, w, h);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, w / (float)h, 0.1f, 5000.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	camera_view(elapsed, w, h);

	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	//glTranslatef(20, 0, 0);
	dei.render_ntexture();

	//glTranslatef(-20, 0, 0);
	
	chair.render();
	colorCube.render();
	sphere.render();



	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	

	glDepthFunc(GL_LESS);

	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	chair.render();
	colorCube.render();
	sphere.render();

	glTranslatef(20, 0, 0);
	dei.render_ntexture();


	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, renderedTexture);


	glRotatef(45, 1, 0, 0);
	glTranslatef(0, 10, 0);
	glBegin(GL_TRIANGLES);
		glTexCoord2f(1, 0);
		glVertex3f(0, 30, 0);
		glTexCoord2f(1, 1);
		glVertex3f(0, 30, 10);
		glTexCoord2f(0, 0);
		glVertex3f(10, 30, 0);

		glTexCoord2f(0, 1);
		glVertex3f(10, 30, 10);
		glTexCoord2f(1, 1);
		glVertex3f(0, 30, 10);
		glTexCoord2f(0, 0);
		glVertex3f(10, 30, 0);
	glEnd();
	


}

