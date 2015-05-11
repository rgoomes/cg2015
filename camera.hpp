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
#define NO_GAME_STATE 0

#define GAME_STATE1 			1
#define GAME_STATE1_ANGLE 		50
#define GAME_STATE1_DEF_ANGLE 	-30
#define GAME_STATE1_DEF_HEIGHT 	47
#define GAME_STATE1_HEIGHT_DROP 8

using namespace std;

class Camera{

	public:
		Camera(int w, int h);

	public:
		int w, h;
		void update(GLFWwindow* window, float elapsed);
		pair<btVector3, btVector3> mouse_update(GLFWwindow* window);

		void lookat(btVector3 dir, btVector3 right);
		void move(GLFWwindow* window, btVector3 p1, btVector3 p2);
		
		void free_camera(GLFWwindow* window, float elapsed);
		void game_camera(GLFWwindow* window, float elapsed);

};

#endif
