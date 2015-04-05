#include <stdio.h>
#include <iostream>
#include <math.h>

#include <GL/glew.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>

#include "display.hpp"
#include "nvidia.hpp"
#include "shaders.hpp"
#include "object.hpp"
#include "misc.hpp"

int SCREEN_WIDTH;
int SCREEN_HEIGHT;

void monitor_resolution(int* w, int* h);