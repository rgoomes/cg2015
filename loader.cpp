#include "loader.hpp"

Loader::Loader(){

}

GLuint Loader::get_texture(string path){
	if(textures.find(path) == textures.end()){
		return textures[path] = loadDDS(path.c_str());
	}else{
		return textures[path];
	}

}

GLuint Loader::get_shaders(string pathVertex, string pathFragment){
	if(shaders.find(pair<string, string>(pathVertex, pathFragment)) == shaders.end()){
		return shaders[pair<string, string>(pathVertex, pathFragment)] = load_shaders(pathVertex.c_str(), pathFragment.c_str());
	}else{
		return shaders[pair<string, string>(pathVertex, pathFragment)];
	}
}

