#version 120
uniform mat4 P;
uniform mat4 MV;


uniform vec4 lightPos1;
uniform vec4 lightPos2;
uniform vec3 ka;
uniform vec3 kd;
uniform vec3 ks;
uniform float s;

varying vec4 aNorCam;
varying vec4 aPosCam;

void main()
{
	vec3 eyevec1 = vec3(0,0,0) - vec3(aPosCam);
	eyevec1 = normalize(eyevec1);
	vec3 n1 = normalize(vec3(aNorCam));
	if(dot(n1,eyevec1) < .2){
		gl_FragColor = vec4(0.0f, 0.0f, 0.0f, 1.0);
	}
	else{
		gl_FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0);
	}
	
}