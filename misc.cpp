
#include "misc.hpp"

#include <GL/glu.h>

void mult_matrix(float r[4][4], float a[4][4], float b[4][4]){
	int i, j, k;
	for(i=0; i<4; i++){
		for(j=0; j<4; j++){
			r[i][j] = 0;
			for(k=0; k<4; k++){
				r[i][j] += a[i][k] * b[k][j];
			}
		}
	}
}

void mult_matrix(float r[4][4], float a[16], float b[4][4]){
	int i, j, k;
	for(i=0; i<4; i++){
		for(j=0; j<4; j++){
			r[i][j] = 0;
			for(k=0; k<4; k++){
				r[i][j] += a[i*4+k] * b[k][j];
			}
		}
	}
}

void print_matrix(float a[]){
	printf("%f %f %f %f\n", a[0], a[1], a[2], a[3]);
	printf("%f %f %f %f\n", a[4+0], a[4+1], a[4+2], a[4+3]);
	printf("%f %f %f %f\n", a[8+0], a[8+1], a[8+2], a[8+3]);
	printf("%f %f %f %f\n", a[12+0], a[12+1], a[12+2], a[12+3]);
	printf("\n");
}