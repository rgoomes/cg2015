
#include "display.hpp"

Object *colorCube;
Rigidbody *sphere;
Rigidbody *chair;
Object *dei;
Rigidbody *dei_collider;
Rigidbody *box;
Object *sphere_aim;
Rigidbody *brick;

float mvp[4][4], a=0;
GLFWwindow* window;
World* world;
Loader* loader;
int ball_count = 0;

GLuint menu[2];
int btn_size_x, btn_size_y;

#define BUTTON_WIDTH  305.0
#define BUTTON_HEIGHT 100.0

#define IMAGE_WIDTH  2560.0
#define IMAGE_HEIGHT 1440.0

void set_environment(GLFWwindow* _window, World* _world, Loader* _loader){
	window = _window;
	world = _world;
	loader = _loader;
}

void load_objects(){
	int w, h;
	glfwGetWindowSize(window, &w, &h);

	for(int j=0; j<10; j++){
		for(int i=0; i<15; i++){
			brick = new Rigidbody("objects/brick", 30, btVector3(-5+2.42*j + (i%2)*1.25, 0.64+1.32*i , 25 * 0.03 ), CUBE, 1.212, 0.66, 0.5005);
			brick->attach_loader(loader);
			brick->set_scale(0.1);
			brick->load_obj();
			world->addObject(brick);
		}
	}

	box = new Rigidbody("objects/box", 0, btVector3(0, 2.0, 30), CONCAVE);
	box->attach_loader(loader);
	box->set_scale(0.2);
	box->load_obj();
	world->addObject(box);

	box = new Rigidbody("objects/box", 0, btVector3(203, 2.7, -56), CONCAVE);
	box->attach_loader(loader);
	box->set_scale(0.2);
	box->load_obj();
	box->rotate(btVector3(0, 1, 0), 45);
	box->rotate(btVector3(1, 0, 0), 5);
	//box->rotate(btVector3(0, 0, 1), 0);
	world->addObject(box);


	dei = new Object("objects/dei");
	dei->attach_loader(loader);
	dei->set_scale(0.1);
	dei->load_obj();
	world->addObject(dei);

	dei_collider = new Rigidbody("objects/dei_collider", 0, btVector3(0, 0, 0), CONCAVE);
	dei_collider->attach_loader(loader);
	dei_collider->set_scale(0.1);
	dei_collider->load_obj();
	world->addCollider(dei_collider);

	/*
	Rigidbody* cylinder = new Rigidbody("objects/cylinder", 5, btVector3(30,0,0));
	cylinder->attach_loader(loader);
	cylinder->set_scale(10);
	cylinder->load_obj();
	world->addObject(cylinder);
	*/

	btVector3 obs_pos = world->camera->get_obs_pos();
	sphere_aim = new Object("objects/sphere");
	sphere_aim->move(obs_pos.getX(), obs_pos.getY(), obs_pos.getZ());
	sphere_aim->attach_loader(loader);
	sphere_aim->set_scale(0.05);
	sphere_aim->load_obj();
	sphere_aim->is_static = true;
	world->addObject(sphere_aim);


}

void load_textures(){
	load_skybox();

	menu[0] = loadDDS("objects/menu/play.dds");
	menu[1] = loadDDS("objects/menu/exit.dds");
}

void add_lights(){
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void throw_ball(){
	btVector3 obs_pos = world->camera->get_obs_pos();
	btVector3 n = world->camera->get_direction();

	n.normalize();
	sphere = new Rigidbody("objects/sphere", 5, btVector3(obs_pos.getX() + n.getX()*10 , obs_pos.getY() + n.getY()*10, obs_pos.getZ() + n.getZ()*10 ), CONVEX, 0,0,0, 0.80);
	sphere->attach_loader(loader);
	sphere->set_scale(0.05);
	sphere->load_obj();
	world->addObject(sphere);

	btRigidBody* r = sphere->get_rigidbody();
	r->setLinearVelocity(world->camera->get_direction() * 100);
	ball_count++;
}

void get_mvp(float mvp[4][4]){
	float m1[4][4], m2[4][4], t[4][4];
	glGetFloatv(GL_MODELVIEW_MATRIX, m1[0]);
	glGetFloatv(GL_PROJECTION_MATRIX, m2[0]);
	mult_matrix(mvp, m1, m2);
}

int last_throw_state = GLFW_RELEASE;
bool throwing = false;

void request_throw(){	
	if(throwing)
		throw_ball();
	
	int cur_state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) | glfwGetKey(window, GLFW_KEY_T);
	if(cur_state == GLFW_PRESS && cur_state != last_throw_state && world->camera->get_game_state() != POINT_TO_POINT)
		throwing = true;
	else
		throwing = false;
	last_throw_state = cur_state;
}

void enable2d(int w, int h){
	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);
	glDisable(GL_LIGHTING);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0,w,0,h);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();
	glUseProgram(0);
}

void disable2d(){
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopAttrib();
	glPopMatrix();
}

void draw_button(int pos, int w, int h){
	glBindTexture(GL_TEXTURE_2D, menu[pos]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glBegin(GL_QUADS);
		glTexCoord2f(0, 1); glVertex2f(0, 0);
		glTexCoord2f(1, 1); glVertex2f(w, 0);
		glTexCoord2f(1, 0); glVertex2f(w, h);
		glTexCoord2f(0, 0); glVertex2f(0, h);
	glEnd();
}

void draw_menu(int w, int h){
	glUseProgram(0);
	glPushMatrix();

	glPushAttrib(GL_ENABLE_BIT);
	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);

	double mouse_x, mouse_y;
	glfwGetCursorPos(window, &mouse_x, &mouse_y);

	double play_size=1, exit_size=1;
	double ypos = h/2.0+h/2.3, play_x = w/2.0+w/3.5, exit_x=w/2.0+w/2.4;
	if(mouse_x > play_x-btn_size_x && mouse_x < play_x+btn_size_x && mouse_y > ypos-btn_size_y && mouse_y < ypos+btn_size_y){
		if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS)
			world->camera->change_state(window, btVector3(196.589996, 54.5, 82.272415));
		play_size=1.05;
	}
	if(mouse_x > exit_x-btn_size_x && mouse_x < exit_x+btn_size_x && mouse_y > ypos-btn_size_y && mouse_y < ypos+btn_size_y){
		if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS)
			exit(0);
		exit_size=1.05;
	}

	glPushMatrix();
		glTranslatef(w/3.5, -h/2.3, 0);
		glTranslatef((-(w*play_size-w))/2.0, (-(h*play_size-h))/2.0 , 0);
		glScalef(play_size, play_size, 1);
		draw_button(0, w,h);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(w/2.4, -h/2.3, 0);
		glTranslatef((-(w*exit_size-w))/2.0, (-(h*exit_size-h))/2.0 , 0);
		glScalef(exit_size, exit_size, 1);
		draw_button(1, w,h);
	glPopMatrix();

	glPopAttrib();
	glPopMatrix();
}

void init_sizes(int w, int h){
	btn_size_x = w * BUTTON_WIDTH  / IMAGE_WIDTH  / 2.0;
	btn_size_y = h * BUTTON_HEIGHT / IMAGE_HEIGHT / 2.0;
}

void timer_update(int w, int h){
	if(world->timer->ticking()){
		// TODO: DISPLAY ELAPSED TIME

		enable2d(w, h);
		printf("\033[A\033[2KElapsed: %.2lfsec\n", world->timer->elapsed());
		disable2d();
	} else if(world->camera->get_game_state() == GAME_STATE1 || 
			  world->camera->get_game_state() == GAME_STATE2)
	
		world->timer->start();
}

void display(float elapsed){
	int w, h;
	glfwGetWindowSize(window, &w, &h);

	timer_update(w, h);
	world->update(elapsed);

	if(world->camera->get_game_state() != NO_GAME_STATE){
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		request_throw();
	} else {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

		enable2d(w, h);
		draw_menu(w, h);
		disable2d();
	}
}
