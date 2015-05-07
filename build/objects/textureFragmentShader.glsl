#version 120

// Interpolated values from the vertex shaders
varying vec2 UV;
varying vec3 Normal_cameraspace;
varying vec3 LightDirection_cameraspace;
varying vec3 LightDirection_tangentspace;
varying vec4 ShadowCoord;
varying vec3 vertPos;
varying vec3 vertexTangent_modelspace;
varying vec3 vertexBitangent_modelspace;

// Values that stay constant for the whole mesh.
uniform sampler2D myTextureSampler;
uniform sampler2D bumpSampler;
uniform sampler2DShadow shadowMap;
uniform mat4 V;
uniform int has_texture;
uniform int has_bump;
uniform float Ns, Tf;

vec2 poissonDisk[16] = vec2[]( 
   vec2( -0.94201624, -0.39906216 ), 
   vec2( 0.94558609, -0.76890725 ), 
   vec2( -0.094184101, -0.92938870 ), 
   vec2( 0.34495938, 0.29387760 ), 
   vec2( -0.91588581, 0.45771432 ), 
   vec2( -0.81544232, -0.87912464 ), 
   vec2( -0.38277543, 0.27676845 ), 
   vec2( 0.97484398, 0.75648379 ), 
   vec2( 0.44323325, -0.97511554 ), 
   vec2( 0.53742981, -0.47373420 ), 
   vec2( -0.26496911, -0.41893023 ), 
   vec2( 0.79197514, 0.19090188 ), 
   vec2( -0.24188840, 0.99706507 ), 
   vec2( -0.81409955, 0.91437590 ), 
   vec2( 0.19984126, 0.78641367 ), 
   vec2( 0.14383161, -0.14100790 ) 
);

float random(vec3 seed, int i){
	vec4 seed4 = vec4(seed, i);
	float dot_product = dot(seed4, vec4(12.9898,78.233,45.164,94.673));
	return fract(sin(dot_product) * 43758.5453);
}

void main(){

	// Light emission properties
	vec3 LightColor = vec3(1,1,1);
	// Material properties
	vec3 MaterialDiffuseColor;
	if(has_texture != 0)
		MaterialDiffuseColor = texture2D( myTextureSampler, UV ).rgb;
	else
		MaterialDiffuseColor = vec3(0.6, 0.6, 0.6);
	vec3 MaterialAmbientColor = 0.4 * MaterialDiffuseColor;
	vec3 MaterialSpecColor = vec3(0.7, 0.7, 0.7);

	vec3 TextureNormal_tangentspace = normalize(texture2D( bumpSampler, vec2(UV.x,-UV.y) ).rgb*2.0 - 1.0);
	//vec3 n = normalize(Normal_cameraspace);
	vec3 n;
	if(has_bump != 0)
		n = normalize(TextureNormal_tangentspace);
	else
		n = normalize(Normal_cameraspace);
	vec3 l = normalize(LightDirection_cameraspace);
	
	float visibility=1.0;
	float cosTheta = clamp( dot( n,-l ), 0,1 );

	float specular = 0.0;
	//if(dot(n, -l) > 0.0){
		vec3 viewDir = normalize(-vertPos);
		specular = pow(max(0.0, dot(reflect(l, n), viewDir)), Ns);
	//}

	//float bias = 0.005;
	// ...variable bias
	float bias = 0.005*tan(acos(cosTheta)); bias = clamp(bias, 0,0.01);
	
	// Sample the shadow map 4 times
	for (int i=0;i<4;i++){
		
		//int index = int(mod(int(16.0*random(Normal_cameraspace.yzx, i)), 16));
		//if(mod(index, 2) == 0)
		int	index = i;
		visibility -= 0.2*(1.0-shadow2D( shadowMap, vec3(ShadowCoord.xy + poissonDisk[index]/2000.0,  (ShadowCoord.z-bias)/ShadowCoord.w )).r);
	}
	if(ShadowCoord.x < 0 || ShadowCoord.x > 1 || ShadowCoord.y < 0 || ShadowCoord.y > 1)
		visibility = 1;

	gl_FragColor.rgb = 
		// Ambient : simulates indirect lighting
		MaterialAmbientColor +
		// Diffuse : "color" of the object
		visibility * MaterialDiffuseColor * LightColor * cosTheta + 
		// Specular: causes intel bug
		visibility*visibility * specular * MaterialSpecColor;

	gl_FragColor.a = Tf;
	

}
