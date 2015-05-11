#include <stdio.h>
#include "camera.hpp"

#define PI 3.141592653

btVector3 game1_pos(149.09, 0,  0);
btVector3 game2_pos(0, 		0,  0); // TODO
btVector3 game3_pos(0, 		0,  0); // TODO
int last_state, game_state = NO_GAME_STATE;

double xpos, ypos, acc;
double horizontal_ang = PI/2.5, vertical_ang = -PI;
bool ftick = true, updated, trigged = false;

btVector3 obs_pos(149.09, 47, 0);
double angle_game  = GAME_STATE1_DEF_ANGLE;
double height_game = GAME_STATE1_DEF_HEIGHT;

void update_state(float elapsed, int key_state){
	angle_game  += elapsed * key_state*GAME_STATE1_ANGLE;
	height_game -= elapsed * key_state*GAME_STATE1_HEIGHT_DROP;

	acc = ACC_DEFAULT;
	last_state = key_state;
	trigged = updated = true;
}

// TODO: MOVE FROM POINT1 TO POINT2 SMOOTHLY
void Camera::move(GLFWwindow* window, btVector3 p1, btVector3 p2){
	pair<btVector3, btVector3> p = this->mouse_update(window);
}

pair<btVector3, btVector3> Camera::mouse_update(GLFWwindow* window){
	if(glfwGetWindowAttrib(window, GLFW_FOCUSED)){
		glfwGetCursorPos(window, &xpos, &ypos);
		glfwSetCursorPos(window, this->w/2, this->h/2);

		if(xpos != 0 && ypos != 0)
			ftick = false;
		if(!ftick){
			horizontal_ang += mouse_speed * double(w/2 - xpos);
			vertical_ang   -= mouse_speed * double(h/2 - ypos);
		}
	}

	btVector3 dir(cos(vertical_ang)*sin(horizontal_ang), sin(vertical_ang), cos(vertical_ang)*cos(horizontal_ang));
	btVector3 right(sin(horizontal_ang - PI/2.0f), 0, cos(horizontal_ang - PI/2.0f));

	return make_pair(dir, right);
}

void Camera::lookat(btVector3 dir, btVector3 right){
	btVector3 tmp = obs_pos+dir;
	btVector3 up = -dir.cross(right);

	gluLookAt(obs_pos.getX(),obs_pos.getY(),obs_pos.getZ(), 
			      tmp.getX(),    tmp.getY(),    tmp.getZ(), 
			      -up.getX(),    -up.getY(),    -up.getZ()
	);
}

void Camera::game_camera(GLFWwindow* window, float elapsed){
	pair<btVector3, btVector3> p = this->mouse_update(window);

	updated = false;
	if(glfwGetKey(window, GLFW_KEY_UP)    || glfwGetKey(window, GLFW_KEY_W))
		update_state(elapsed, 1);
	if(glfwGetKey(window, GLFW_KEY_DOWN)  || glfwGetKey(window, GLFW_KEY_S))
		update_state(elapsed, -1);
	if(!updated && trigged){
		if(acc > 0){
			angle_game  += elapsed * last_state*acc*GAME_STATE1_ANGLE;
			height_game -= elapsed * last_state*acc*GAME_STATE1_HEIGHT_DROP;
			acc 		-= ACC_DROP;
		} else {
			trigged = false;
			acc 	= ACC_DEFAULT;
		}
	}

	obs_pos.setX(game1_pos.getX() - sin(angle_game*PI/180)*95);
	obs_pos.setY(game1_pos.getY() + height_game);
	obs_pos.setZ(game1_pos.getZ() + cos(angle_game*PI/180)*95);

	this->lookat(p.first, p.second);
}

void Camera::free_camera(GLFWwindow* window, float elapsed){
	pair<btVector3, btVector3> p = this->mouse_update(window);

	if(glfwGetKey(window, GLFW_KEY_UP)    || glfwGetKey(window, GLFW_KEY_W))
		obs_pos += p.first * elapsed * speed;
	if(glfwGetKey(window, GLFW_KEY_DOWN)  || glfwGetKey(window, GLFW_KEY_S))
		obs_pos -= p.first * elapsed * speed;
	if(glfwGetKey(window, GLFW_KEY_RIGHT) || glfwGetKey(window, GLFW_KEY_D))
		obs_pos -= p.second * elapsed * speed;
	if(glfwGetKey(window, GLFW_KEY_LEFT)  || glfwGetKey(window, GLFW_KEY_A))
		obs_pos += p.second * elapsed * speed;

	this->lookat(p.first, p.second);
}

void Camera::update(GLFWwindow* window, float elapsed){

	if(game_state == NO_GAME_STATE)
		this->free_camera(window, elapsed);
	else if(game_state == GAME_STATE1)
		this->game_camera(window, elapsed);
}

Camera::Camera(int w, int h){
	this->w = w;
	this->h = h;
}
