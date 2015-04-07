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

void load_objects();
void add_lights();
void display(GLFWwindow* window, float elapsed);

#endif