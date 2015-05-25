#include <stdio.h>
#include <iostream>
#include "camera.hpp"

#define PI 3.141592653

int last_state;

double xpos, ypos, acc;
double horizontal_ang = -PI/1.25, vertical_ang = -PI;
bool ftick = true, updated, trigged = false;

double angle_game  = GAME_STATE1_DEF_ANGLE;
double height_game = GAME_STATE1_DEF_HEIGHT;

void update_state(float elapsed, int key_state){
	angle_game  += elapsed * key_state*GAME_STATE1_ANGLE;
	height_game -= elapsed * key_state*GAME_STATE1_HEIGHT_DROP;

	acc = ACC_DEFAULT;
	last_state = key_state;
	trigged = updated = true;
}

void Camera::move(GLFWwindow* window, float elapsed, btVector3 dest){
	pair<btVector3, btVector3> p = this->mouse_update(window);

	if((dest - obs_pos).norm() < 0.5)
		this->camera_state = NO_GAME_STATE;
	else{
		btVector3 dif = (dest - obs_pos).normalize();
		float t = (dest - obs_pos).norm() / 10.0 + 5;
		obs_pos += dif * elapsed * t*t;
	}

	this->lookat(p.first, p.second);
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

btVector3 Camera::get_direction(){
	return btVector3(cos(vertical_ang)*sin(horizontal_ang), sin(vertical_ang), cos(vertical_ang)*cos(horizontal_ang));
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

	if(glfwGetKey(window, GLFW_KEY_F))
		this->camera_state = POINT_TO_POINT;
	else if(glfwGetKey(window, GLFW_KEY_G))
		this->camera_state = GAME_STATE1;

	this->lookat(p.first, p.second);
}

void Camera::update(GLFWwindow* window, float elapsed){

	if(this->camera_state == NO_GAME_STATE)
		this->free_camera(window, elapsed);
	else if(this->camera_state == GAME_STATE1)
		this->game_camera(window, elapsed);
	else if(this->camera_state == POINT_TO_POINT)
		this->move(window, elapsed, btVector3(0, 30, 0));
}

Camera::Camera(int w, int h){
	no_game = btVector3(-13.7, 10.6, -142);
	game1_pos = btVector3(149.09, 7.5,  0);
	game2_pos = btVector3(0, 0, 0); // TODO
	game3_pos = btVector3(0, 0, 0); // TODO
	obs_pos = no_game;

	this->camera_state = NO_GAME_STATE;

	this->w = w;
	this->h = h;
}

btVector3 Camera::get_obs_pos() {
	return obs_pos;
}