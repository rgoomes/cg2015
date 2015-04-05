#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/freeglut.h>

void display_text(char *string, GLfloat x, GLfloat y){
	glRasterPos2f(x, y);

	while(*string)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *string++);
}

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