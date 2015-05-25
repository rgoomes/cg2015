#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include <math.h>
#include <GLFW/glfw3.h>
#include <GL/glu.h>
#include <btBulletDynamicsCommon.h>

#include "camera.hpp"
#include "misc.hpp"
#include "skybox.hpp"
#include "timer.hpp"

#define SHADOW_RES 1024

using namespace std;

class Object;
class Rigidbody;

class World{
	public:
		void addObject(Object* obj);
		void addCollider(Object* obj);
		void update(float elapsed);
		World(GLFWwindow* window);
		Camera* camera;
		Timer* timer;

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
};

#endif