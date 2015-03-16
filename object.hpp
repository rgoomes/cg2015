#include <iostream>
#include <vector>
#include <string>
#include <string.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/freeglut.h>

using std::vector;
using std::string;

typedef struct {
	GLfloat x, y, z;
	GLfloat w = 1.0;
} Point;

typedef struct {
	GLint v_index[3];
	GLint t_index[3];
	GLint n_index[3];
} Face;

class Object {

	public:
		string path;

		vector<Point> vertices, normals, uvs;
		vector<Face> faces;

		Object(string path);
		~Object();

		bool load_obj(bool debug);
};
