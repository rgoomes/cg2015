#ifndef LOADER_H
#define LOADER_H


#include <map>
#include <vector>
#include <utility>

#include "image.hpp"
#include "shaders.hpp"

using namespace std;

class Loader{
	public:
		Loader();
		~Loader();
		GLuint get_texture(string path);
		GLuint get_shaders(string pathVertex, string pathFragment);
	private:
		map<string, GLuint> textures;
		map<pair<string, string>, GLuint> shaders;
};

#endif