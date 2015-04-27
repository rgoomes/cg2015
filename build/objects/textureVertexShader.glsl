#version 120

// Input vertex data, different for all executions of this shader.
attribute vec3 vertexPosition_modelspace;
attribute vec2 vertexUV;
attribute vec3 vertexNormal_modelspace;

// Output data ; will be interpolated for each fragment.
varying vec2 UV;
varying vec3 vertPos;
varying vec4 ShadowCoord;
varying vec3 LightDirection_cameraspace;
varying vec3 Normal_cameraspace;

// Values that stay constant for the whole mesh.
uniform mat4 MVP;
uniform mat4 DepthBiasMVP;
uniform mat4 V;
uniform mat4 M;
uniform vec3 LightInvDirection_worldspace;
uniform int has_texture;

void main(){
	// Output position of the vertex, in clip space : MVP * position
	
	vertPos = normalize(V * M * vec4(vertexPosition_modelspace+vec3(0,0,0), 1.0)).xyz;

	LightDirection_cameraspace = (V * vec4(-LightInvDirection_worldspace, 0)).xyz;
	Normal_cameraspace = (V * M * vec4(vertexNormal_modelspace, 0)).xyz;

	gl_Position =  MVP * vec4(vertexPosition_modelspace,1);
	ShadowCoord = DepthBiasMVP * vec4(vertexPosition_modelspace,1);
	
	// UV of the vertex. No special space for this one.
	UV = vertexUV;
}

