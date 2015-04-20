#ifndef DISPLAY_H
#define DISPLAY_H

#include <math.h>
#include <GL/glew.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>
#include <btBulletDynamicsCommon.h>
#if (BT_BULLET_VERSION == 282)
	inline int bullet_btInfinityMask(){return btInfinityMask;}
#endif

#include "misc.hpp"
#include "object.hpp"
#include "rigidbody.hpp"
#include "world.hpp"

void load_textures();
void load_objects();
void add_lights();
void set_environment(GLFWwindow* _window, World* _world);
void display(float elapsed);
void get_render_buffer();

#endif