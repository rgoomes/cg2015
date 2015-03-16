#include "object.hpp"
#include "nvidia.hpp"

int main(){
	Object obj("tests/cube.obj");
	obj.load_obj(true);
	
	return 0;
}
