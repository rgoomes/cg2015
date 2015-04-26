#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <iostream>
#include <math.h>

#include <GL/glew.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>
#include <btBulletDynamicsCommon.h>

#include "display.hpp"
#include "nvidia.hpp"
#include "misc.hpp"
#include "world.hpp"

#ifdef __unix
	#define CONSOLE_FPS 1
#endif

#define DISPLAY_FPS 1

using namespace std;

int SCREEN_WIDTH;
int SCREEN_HEIGHT;

void monitor_resolution(int* w, int* h);

GLFWwindow* main_window;
World* main_world;
Loader* main_loader;
//extern Rigidbody chair;

#endif
