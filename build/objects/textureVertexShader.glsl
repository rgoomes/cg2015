#version 120

// Input vertex data, different for all executions of this shader.
attribute vec3 vertexPosition_modelspace;
attribute vec2 vertexUV;
attribute vec3 vertexNormal_modelspace;

// Output data ; will be interpolated for each fragment.
varying vec2 UV;
varying vec4 ShadowCoord;
varying vec3 LightDirection_cameraspace;
varying vec3 Normal_cameraspace;
varying vec3 vertPos;

// Values that stay constant for the whole mesh.
uniform mat4 MVP;
uniform mat4 DepthBiasMVP;
uniform mat4 V;
uniform mat4 M;
uniform vec3 LightInvDirection_worldspace;

void main(){
	// Output position of the vertex, in clip space : MVP * position
	gl_Position =  MVP * vec4(vertexPosition_modelspace,1);
	ShadowCoord = DepthBiasMVP * vec4(vertexPosition_modelspace,1);
	
	vec4 vertPos4 = MVP * vec4(vertexPosition_modelspace, 1.0);
	vertPos = vec3(vertPos4) / vertPos4.w; 

	LightDirection_cameraspace = (V * vec4(LightInvDirection_worldspace, 0)).xyz;
	//Normal_cameraspace = (V * vec4(vertexNormal_modelspace, 0)).xyz;
	Normal_cameraspace = (V * M * vec4(vertexNormal_modelspace, 0)).xyz;

	// UV of the vertex. No special space for this one.
	UV = vertexUV;
}

