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
#include "world.hpp"
#include "rigidbody.hpp"
#include "grass.hpp"
#include "loader.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H

class World;
class Object;
class Rigidbody;

void load_textures();
void load_objects();
void add_lights();
void set_environment(GLFWwindow* _window, World* _world, Loader* _loader);
void display(float elapsed);
void get_render_buffer();
void init_sizes(int w, int h);
void init_freetype();

#endif