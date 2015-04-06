#ifndef DISPLAY_H
#define DISPLAY_H

#include <btBulletDynamicsCommon.h>
#if (BT_BULLET_VERSION == 282)
	inline int bullet_btInfinityMask(){return btInfinityMask;}
#endif

#include <math.h>

#include <GL/glew.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>

#define DISPLAY_FPS 0
#define CONSOLE_FPS 1

void load_objects();
void add_lights();
void display(GLFWwindow* window);

#endif