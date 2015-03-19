#include <vector>
#include <string>

#include "types.hpp"

using namespace std;

class Object {

	public:
		string path;

		vector<Point> vertices, normals, uvs;
		vector<Point> out_vertices;
		vector<Face> faces;

		Object(string path);
		~Object();

		bool load_obj(bool debug);
};
