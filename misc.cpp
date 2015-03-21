#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/freeglut.h>

void display_text(char *string, GLfloat x, GLfloat y){
	glRasterPos2f(x, y);

	while(*string)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *string++);
}