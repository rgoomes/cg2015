#ifndef SKYBOX_H
#define SKYBOX_H

#include <GL/gl.h>
#include <string>

#include "image.hpp"

using std::string;

void load_skybox();
void draw_skybox(double D);
void draw_skyface(int pos, int *d, double D);

#endif
