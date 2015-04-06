#version 120

// Input vertex data, different for all executions of this shader.
attribute vec3 vertexPosition_modelspace;
attribute vec2 vertexUV;

// Output data ; will be interpolated for each fragment.
varying vec2 UV;

varying vec3 temp;

// Values that stay constant for the whole mesh.
uniform mat4 MVP;

void main(){

	// Output position of the vertex, in clip space : MVP * position
	temp = vertexPosition_modelspace + vec3(3, 0, 0);
	gl_Position =  MVP * vec4(temp,1);
	
	// UV of the vertex. No special space for this one.
	UV = vertexUV;
}

