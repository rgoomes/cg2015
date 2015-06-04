#ifndef CAMERA_H
#define CAMERA_H

#include <GLFW/glfw3.h>
#include <GL/glu.h>
#include <btBulletDynamicsCommon.h>
#include <utility>

#define speed 10.0f
#define mouse_speed 0.002f

#define ACC_DROP 0.03
#define ACC_DEFAULT 1

#define POINT_TO_POINT -1
#define FREE_CAMERA 0
#define NO_GAME_STATE 1

#define GAME_STATE1 			2
#define GAME_STATE1_ANGLE 		50
#define GAME_STATE1_DEF_ANGLE 	-30
#define GAME_STATE1_DEF_HEIGHT 	47
#define GAME_STATE1_HEIGHT_DROP 8

#define GAME_STATE2 			3

#define CAMERA_MIN_Y 			0
#define CAMERA_MAX_Y			100

using namespace std;

class Camera{
	int camera_state;

	public:
		Camera(int w, int h);
		btVector3 destination;
		btVector3 no_game;
		btVector3 game1_pos;
		btVector3 game2_pos; // TODO
		btVector3 game3_pos; // TODO
		btVector3 obs_pos = no_game;
		double horizontal_ang, vertical_ang;

	public:
		int w, h;
		void update(GLFWwindow* window, float elapsed);
		pair<btVector3, btVector3> mouse_update(GLFWwindow* window);

		void lookat(btVector3 dir, btVector3 right);
		void move(GLFWwindow* window, float elapsed, btVector3 dest);
		
		void start(GLFWwindow* window, float elapsed);
		void free_camera(GLFWwindow* window, float elapsed);
		void game_camera1(GLFWwindow* window, float elapsed);
		void game_camera2(GLFWwindow* window, float elapsed);
		void change_state(btVector3 dest);

		btVector3 get_obs_pos();
		btVector3 get_direction();
		btVector3 get_right();
		int get_game_state();

};


#endif
