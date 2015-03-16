#include "object.h"
#include "nvidia.h"

int main(){
	Object obj("tests/cube.obj");
	obj.load_obj(true);
	
	return 0;
}
