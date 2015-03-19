#define SCREEN_WIDTH  glutGet(GLUT_SCREEN_WIDTH)
#define SCREEN_HEIGHT glutGet(GLUT_SCREEN_HEIGHT)

GLfloat r,g,b;

Object cube("tests/cube.obj");

void time_update(int value){
	glutTimerFunc(50, time_update, 0);
	glutPostRedisplay();
}

void display(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for(int i = 0; i < cube.out_vertices.size(); i += 3){
		glBegin(GL_POLYGON);
			glVertex3f(cube.out_vertices[i+0].x, cube.out_vertices[i+0].y, cube.out_vertices[i+0].z);
			glVertex3f(cube.out_vertices[i+1].x, cube.out_vertices[i+1].y, cube.out_vertices[i+1].z);
			glVertex3f(cube.out_vertices[i+2].x, cube.out_vertices[i+2].y, cube.out_vertices[i+2].z);
		glEnd();
	}
	
	glutSwapBuffers();
}

void load_objects(){
	cube.load_obj(true);
}

void opengl_init(int argc, char **argv){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	glutCreateWindow("cg2015");

	load_objects();
	glClearColor(0.3,0.4,0.5, 1);
	
	glMatrixMode(GL_PROJECTION);
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	gluPerspective(90, SCREEN_WIDTH/SCREEN_HEIGHT, 0, 10);
	glMatrixMode(GL_MODELVIEW);
	gluLookAt(5,5,5, 0,0,0, 0,1,0);
	glShadeModel(GL_SMOOTH);

	/////////////////////////////////////////////////////
	/////////////////////////////////////////////////////

	glutDisplayFunc(display);
	glutTimerFunc(1, time_update, 0);
}