#include <stdio.h>
#include <iostream>
#include <math.h>

#include <GL/glew.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>
#include <btBulletDynamicsCommon.h>

#include "display.hpp"
#include "nvidia.hpp"
#include "shaders.hpp"
#include "object.hpp"
#include "misc.hpp"

#define DISPLAY_FPS 1
#define CONSOLE_FPS 1

int SCREEN_WIDTH;
int SCREEN_HEIGHT;

void monitor_resolution(int* w, int* h);
