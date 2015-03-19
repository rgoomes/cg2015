#include <stdio.h>

#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <math.h>

#include "object.hpp"
#include "nvidia.hpp"

using namespace std;

bool keys_states[256];
bool keys_states_std[256];

GLfloat r, g ,b;
GLfloat angle = 0, angle2 = 0;
GLdouble pos_x=1.5, pos_y, pos_z=1.5, v_x=0, v_y=0, v_z=-0.2;
Object obj("tests/cube.obj");

void kbs(int key, int x, int y)   { keys_states[key] = true; }
void kbs_up(int key, int x, int y){ keys_states[key] = false;}
void kb(unsigned char key, int x, int y)   { keys_states_std[key] = true; /*printf("%d\n", key);*/}
void kb_up(unsigned char key, int x, int y){ keys_states_std[key] = false;}

void key_updates(){
	if(keys_states[GLUT_KEY_LEFT]){
		angle -= 0.05;
	}
	if(keys_states[GLUT_KEY_RIGHT]){
		angle += 0.05;
	}
	if(keys_states[GLUT_KEY_DOWN]){
		angle2 += 0.05;
	}
	if(keys_states[GLUT_KEY_UP]){
		angle2 -= 0.05;
	}
	if(keys_states_std['w']){
		pos_x += v_x*0.1;
		pos_y += v_y*0.1;
		pos_z += v_z*0.1;
	}
	if(keys_states_std['s']){
		pos_x -= v_x*0.1;
		pos_y -= v_y*0.1;
		pos_z -= v_z*0.1;
	}
	if(keys_states_std['a']){
		pos_x -= -v_x*0.1;
		pos_y -= v_z*0.1;
		pos_z -= v_y*0.1;
	}
	if(keys_states_std['d']){
		pos_x -= v_x*0.1;
		pos_y -= v_y*0.1;
		pos_z -= v_z*0.1;
	}

	if(angle2 > 3.14)
		angle2 -= 3.14;
	if(angle2 < -3.14)
		angle2 += 3.14;

	v_x = cos(angle) * sin(angle2);
	v_z = sin(angle) * sin(angle2);
	v_y = cos(angle2);

}

void time_update(int value){
	glutTimerFunc(45, time_update, 0);
	glutPostRedisplay();
}

void init(){
	pos_x = pos_z = 0;

	glClearColor(0, 0, 0, 1);
	
	glViewport(0,0, 400, 300);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glOrtho(-1,1, -1,1, -1,1);
	gluPerspective(100, 1, 0, 10);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0,1.5,1.5, 0,0,0, 0,1,0);
	
}

void display(){
	key_updates();

	glColor3f(1, 1, 1);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
		glLineWidth(1.0f);
		//glutWireTeapot(0.5);

		glTranslatef(1, 0, 0);
		glutWireCube(0.5);
	glPopMatrix();

	glColor3f(1, 0, 0);

	glPushMatrix();
		glLineWidth(5.0f);
		glBegin(GL_LINE_STRIP);
			glVertex3f(0, 0, 0);
			glVertex3f(0, 0.5, 0);
		glEnd();

		glBegin(GL_LINE_STRIP);
			glVertex3f(0, 0, 0);
			glVertex3f(0.5, 0, 0);
		glEnd();

		glBegin(GL_LINE_STRIP);
			glVertex3f(0, 0, 0);
			glVertex3f(0, 0, 0.5);
		glEnd();
	glPopMatrix();

	/*
	for(int i=0; i<obj.out_vertices.size(); i+=3){
		glBegin(GL_POLYGON);
			glVertex3f(obj.out_vertices[i+0].x, obj.out_vertices[i+0].y, obj.out_vertices[i+0].z);
			glVertex3f(obj.out_vertices[i+1].x, obj.out_vertices[i+1].y, obj.out_vertices[i+1].z);
			glVertex3f(obj.out_vertices[i+2].x, obj.out_vertices[i+2].y, obj.out_vertices[i+2].z);
			//printf("%f %f %f\n", obj.out_vertices[0].x, obj.out_vertices[0].y, obj.out_vertices[0].z);
		glEnd();
	}*/
	glBufferData(GL_ARRAY_BUFFER, obj.out_vertices.size() * 3 * sizeof(Point), &obj.out_vertices[0], GL_STATIC_DRAW);

	glLoadIdentity();

	if(angle2 < 0)
		gluLookAt(pos_x,pos_y,pos_z, pos_x+v_x,pos_y+v_y,pos_z+v_z, 0,-1,0);
	else
		gluLookAt(pos_x,pos_y,pos_z, pos_x+v_x,pos_y+v_y,pos_z+v_z, 0,1,0);
	
	glFlush();
}

int main(int argc, char **argv){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1500, 1500);
	glutInitWindowPosition(1366/2 - 500/2, 768/2 - 350 / 2);
	glutCreateWindow("3D");

	init();

	glutKeyboardFunc(kb);
	glutKeyboardUpFunc(kb_up);
	glutSpecialFunc(kbs);
	glutSpecialUpFunc(kbs_up);
	glutDisplayFunc(display);

	obj.load_obj(true);
	glutTimerFunc(1, time_update, 0);
	printf("Load\n");
	glutMainLoop();


	return 0;
}

/*

int main(){
	
	
	return 0;
}
*/