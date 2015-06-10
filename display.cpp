
#include "display.hpp"
#include <time.h>

Object *colorCube;
Rigidbody *sphere;
Rigidbody *chair;
Object *dei;
Rigidbody *dei_collider;
Rigidbody *box, *box2;
Object *sphere_aim;
Rigidbody *brick;
Rigidbody *ramp;

float mvp[4][4], a=0;
GLFWwindow* window;
World* world;
Loader* loader;
int ball_count = 0;

GLuint menu[2];
int btn_size_x, btn_size_y;

GLuint vbo;
FT_Library ft;
FT_Face face;

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

	for(int depth=0; depth<1; depth++){
		for(int j=0; j<10; j++){
			for(int i=0; i<15; i++){
				brick = new Rigidbody("objects/brick", 50, btVector3(-5+2.42*j + (i%2)*1.25, 0.64+1.32*i , 25 * 0.03 + depth * 1.1), CUBE, 1.212, 0.66, 0.5005, 0.7, 0.9);
				brick->attach_loader(loader);
				brick->set_scale(0.1);
				brick->load_obj();
				brick->bumpfix = true;
				world->addObject(brick);
			}
		}
	}

	ramp = new Rigidbody("objects/ramp", 0, btVector3(175, 4, -73.5), CONCAVE);
	ramp->attach_loader(loader);
	ramp->set_scale(0.1);
	ramp->load_obj();
	ramp->rotate(btVector3(0, 1, 0), -120);
	ramp->rotate(btVector3(0, 0, 1), 4);
	ramp->rotate(btVector3(1, 0, 0), -5);
	world->addObject(ramp);

	box = new Rigidbody("objects/box", 0, btVector3(7.5, 3.0, 20), CONCAVE);
	box->attach_loader(loader);
	box->set_scale(0.3);
	box->load_obj();
	box->rotate(btVector3(0, 1, 0), 90);
	world->addObject(box);

	box2 = new Rigidbody("objects/box2", 0, btVector3(204, 2.7, -58.5), CONCAVE);
	box2->attach_loader(loader);
	box2->set_scale(0.2);
	box2->load_obj();
	box2->rotate(btVector3(0, 1, 0), 45);
	box2->rotate(btVector3(1, 0, 0), 5);
	box2->rotate(btVector3(0, 0, 1), 0);
	world->addObject(box2);

	dei = new Object("objects/dei");
	dei->attach_loader(loader);
	dei->set_scale(0.1);
	dei->load_obj();
	world->addObject(dei);

	dei_collider = new Rigidbody("objects/dei_collider", 0, btVector3(0, 0, 0), CONCAVE, 0, 0, 0, 0.4, 0.4);
	dei_collider->attach_loader(loader);
	dei_collider->set_scale(0.1);
	dei_collider->load_obj();
	world->addCollider(dei_collider);

	srand(time(NULL));
	
	float s=7;
	for(int i=-5; i< 100; i+=s){
		for(int j=-2; j< 180; j+=s){
			Grass* grass = new Grass("objects/grass");
			grass->attach_loader(loader);
			grass->set_scale(0.04);
			grass->load_obj();
			grass->set_transparent(true);
			grass->move(28 + i + (s/2) * ((rand() % 100) / 100.0), 0,-90 + j + (s/2) * ((rand() % 100) / 100.0) );
			world->addObject(grass);
		}
	}

	btVector3 obs_pos = world->camera->get_obs_pos();
	sphere_aim = new Object("objects/sphere");
	sphere_aim->move(0,0,0);
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

vector<Rigidbody*> balls;
vector<double> times;

void throw_ball(){
	btVector3 obs_pos = world->camera->get_obs_pos();
	btVector3 n = world->camera->get_direction();
	btVector3 right = world->camera->get_right();

	n.normalize();
	right.normalize();
	btVector3 inc = n - right / 5;
	sphere = new Rigidbody("objects/sphere", 5, btVector3(obs_pos.getX() + inc.getX()*10 , obs_pos.getY() + inc.getY()*10, obs_pos.getZ() + inc.getZ()*10 ), CONVEX, 0,0,0, 0.80);
	sphere->attach_loader(loader);
	sphere->set_scale(0.05);
	sphere->load_obj();
	world->addObject(sphere);

	if(world->camera->get_game_state() != FREE_CAMERA){
		balls.push_back(sphere);
		times.push_back(0);
	}

	btRigidBody* r = sphere->get_rigidbody();
	r->setLinearVelocity( (n + right / 20) * 100);
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
	if(throwing){
		btVector3 box_pos(204, 2.7, -58.5);
		double dist_to_box = (world->camera->obs_pos-box_pos).length();

		if((world->camera->get_game_state() == GAME_STATE1 && dist_to_box > 120)
			|| (world->camera->get_game_state() != GAME_STATE1))

		throw_ball();
	}
	
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

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glEnable(GL_LIGHTING);
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

void render_text(const char *text, float x, float y, float sx, float sy) {
	const char *p;
	FT_GlyphSlot g = face->glyph;

	glUseProgram(0);
	glPushMatrix();

	glPushAttrib(GL_ENABLE_BIT);
	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);

	GLuint tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	for(p=text; *p; p++) {
		if(FT_Load_Char(face, *p, FT_LOAD_RENDER))
			continue;

		glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA8, g->bitmap.width, g->bitmap.rows, 0, GL_ALPHA, GL_UNSIGNED_BYTE, g->bitmap.buffer);
	
		float x2 = x + g->bitmap_left * sx;
	    float y2 = -y - g->bitmap_top * sy;
	    float w = g->bitmap.width * sx;
	    float h = g->bitmap.rows * sy;
	 
	    GLfloat box[4][4] = {
	        {x2,     -y2    , 0, 0},
	        {x2,     -y2 - h, 0, 1},
	        {x2 + w, -y2 - h, 1, 1},
	        {x2 + w, -y2    , 1, 0},
	    };
			
		glBegin(GL_QUADS);
			glTexCoord2f(box[0][2], box[0][3]); glVertex2f(box[0][0], box[0][1]);
			glTexCoord2f(box[1][2], box[1][3]); glVertex2f(box[1][0], box[1][1]);
			glTexCoord2f(box[2][2], box[2][3]); glVertex2f(box[2][0], box[2][1]);
			glTexCoord2f(box[3][2], box[3][3]); glVertex2f(box[3][0], box[3][1]);
			glTexCoord2f(box[0][2], box[0][3]);
		glEnd();
		
		x += (g->advance.x >> 6) * sx;
		y += (g->advance.y >> 6) * sy;
	}

	glDeleteTextures(1, &tex);

	glPopAttrib();
	glPopMatrix();
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

void init_freetype(){
	FT_Init_FreeType(&ft);
	FT_New_Face(ft, "font/FreeSans.ttf", 0, &face);
	FT_Set_Pixel_Sizes(face, 0, 48);
}

int clock_size = 0;
double old_clock;

void print_elapsed(int w, int h, bool print_old){
	enable2d(w, h);

	glColor4f(0,0,0,1);
	char str[8], str_tmp[8];
	sprintf(str, "%.2lf", print_old ? old_clock : world->timer->elapsed());
	sprintf(str_tmp, "%.2lf", old_clock);

	if(strlen(str) > strlen(str_tmp))
		clock_size += 27;
	if(!print_old)
		old_clock = world->timer->elapsed();

	render_text(str, w-100-clock_size, h-50, 1, 1);
	glColor4f(1,1,1,1);
	disable2d();
}

void timer_update(int w, int h){
	if(world->timer->ticking()){
		print_elapsed(w, h, false);

	} else if(world->camera->get_game_state() == GAME_STATE1 || 
			  world->camera->get_game_state() == GAME_STATE2){
	
		world->timer->start();
		print_elapsed(w, h, true);
	}
}

void inside_box(float elapsed){
	for(int i = 0; i < (int)balls.size(); i++){
		bool in_box = box->contains(balls[i], 4.0);
		bool in_box2 = box2->contains(balls[i], 3.0);

		if((world->camera->get_game_state() == GAME_STATE1 && in_box) ||
		   (world->camera->get_game_state() == GAME_STATE2 && in_box2)){

			balls.erase(balls.begin() + i);
			times.erase(times.begin() + i);
		} else if((world->camera->get_game_state() == GAME_STATE1 && in_box2) || 
		   		  (world->camera->get_game_state() == GAME_STATE2 && in_box)){
			
			times[i] += elapsed;

			if(times[i] > 2){ // INSIDE FOR 2 SECONDS
				balls.erase(balls.begin() + i);
				times.erase(times.begin() + i);

				if(world->camera->get_game_state() == GAME_STATE1)
					world->camera->change_state(window, btVector3(0, 20.6, -100));
				else if(world->camera->get_game_state() == GAME_STATE2){
					world->camera->set_camera_state(FREE_CAMERA);
					world->camera->change_state(window, btVector3(-13.7, 10.6, 140));
				}
			}
		} else{
			times[i] = 0;
		}
	}
}

void display(float elapsed){
	int w, h;
	glfwGetWindowSize(window, &w, &h);

	world->update(elapsed);
	timer_update(w, h);
	inside_box(elapsed);

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
