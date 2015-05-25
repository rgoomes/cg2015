#ifndef CAMERA_H
#define CAMERA_H

#include <GLFW/glfw3.h>
#include <GL/glu.h>
#include <btBulletDynamicsCommon.h>
#include <utility>

#define speed 90.0f
#define mouse_speed 0.002f

#define ACC_DROP 0.03
#define ACC_DEFAULT 1
#define POINT_TO_POINT -1
#define NO_GAME_STATE 0

#define GAME_STATE1 			1
#define GAME_STATE1_ANGLE 		50
#define GAME_STATE1_DEF_ANGLE 	-30
#define GAME_STATE1_DEF_HEIGHT 	47
#define GAME_STATE1_HEIGHT_DROP 8

using namespace std;


class Camera{
	int camera_state;

	public:
		Camera(int w, int h);
		btVector3 no_game;
		btVector3 game1_pos;
		btVector3 game2_pos; // TODO
		btVector3 game3_pos; // TODO
		btVector3 obs_pos = no_game;

	public:
		int w, h;
		void update(GLFWwindow* window, float elapsed);
		pair<btVector3, btVector3> mouse_update(GLFWwindow* window);


		void lookat(btVector3 dir, btVector3 right);
		void move(GLFWwindow* window, float elapsed, btVector3 dest);
		
		void free_camera(GLFWwindow* window, float elapsed);
		void game_camera(GLFWwindow* window, float elapsed);
		btVector3 get_obs_pos();
		btVector3 get_direction();
		btVector3 get_right();

};


#endif
