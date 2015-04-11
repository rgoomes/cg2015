#version 120

// Input vertex data, different for all executions of this shader.
attribute vec3 vertexPosition_modelspace;
attribute vec2 vertexUV;

// Output data ; will be interpolated for each fragment.
varying vec2 UV;
varying vec3 temp;

// Values that stay constant for the whole mesh.
uniform mat4 MVP;
uniform vec3 translate;

void main(){

	// Output position of the vertex, in clip space : MVP * position
	gl_Position =  MVP * vec4(vertexPosition_modelspace, 1);
	
	// UV of the vertex. No special space for this one.
	UV = vertexUV;
}

