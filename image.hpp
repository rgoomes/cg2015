#ifndef IMAGE_H
#define IMAGE_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <GL/gl.h>

#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

GLuint loadDDS(const char * imagepath);
GLuint loadBMP(const char * imagepath);

#endif