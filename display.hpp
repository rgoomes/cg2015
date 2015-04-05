#ifndef DISPLAY_H
#define DISPLAY_H

#include <math.h>

#define DISPLAY_FPS 0
#define CONSOLE_FPS 1

void load_objects();
void add_lights();
void display(GLFWwindow* window);

#endif