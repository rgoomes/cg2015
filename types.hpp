#include <GL/gl.h>

typedef struct {
	GLfloat x, y, z;
	GLfloat w = 1.0;
} Point;

typedef struct {
	GLint v_index[3];
	GLint t_index[3];
	GLint n_index[3];
} Face;
