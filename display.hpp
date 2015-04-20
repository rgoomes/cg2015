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
#include "skybox.hpp"

#define WORLD_MIN_X -20 
#define WORLD_MAX_X 230
#define WORLD_MIN_Y 1
#define WORLD_MAX_Y 30
#define WORLD_MIN_Z -145
#define WORLD_MAX_Z 145

void load_textures();
void load_objects();
void add_lights();
void set_environment(GLFWwindow* _window, btDynamicsWorld* _world);
void display(float elapsed);
void get_render_buffer();

#endif