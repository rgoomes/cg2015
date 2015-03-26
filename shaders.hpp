#ifndef SHADERS_H
#define SHADERS_H

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <GL/glext.h>

#include "types.hpp"

using namespace std;

GLuint load_shaders(const char * vertex_file_path, const char * fragment_file_path);

#endif
