#ifndef MISC_H
#define MISC_H

#include <GL/gl.h>
#include <stdio.h>

#define PI 3.141592653

void mult_matrix(float r[4][4], float a[4][4], float b[4][4]);
void mult_matrix(float r[4][4], float a[4][4], float b[16]);
void mult_matrix(float r[4][4], float a[16], float b[4][4]);
void print_matrix(float a[]);

#endif