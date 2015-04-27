#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include <math.h>
#include <GLFW/glfw3.h>
#include <GL/glu.h>
#include <btBulletDynamicsCommon.h>

#include "misc.hpp"
#include "skybox.hpp"

#define WORLD_MIN_X -20 
#define WORLD_MAX_X 230
#define WORLD_MIN_Y 1
#define WORLD_MAX_Y 40
#define WORLD_MIN_Z -145
#define WORLD_MAX_Z 145

#define SHADOW_RES 1024

using namespace std;

class Object;
class Rigidbody;

class World{
	public:
		void addObject(Object* obj);
		void update(float elapsed);
		World(GLFWwindow* window);

	private:
		btDynamicsWorld* physicsWorld;
		vector<Object*> objects;
		GLFWwindow* window;
		int window_width, window_height;
		GLuint renderedTexture;
		GLuint FramebufferName = 0;

		btDynamicsWorld* getDynamicWorld();
		GLuint get_render_buffer();
		void tick(float elapsed);
		void init();
		void camera_view(float elapsed, int w, int h);

};

#endif