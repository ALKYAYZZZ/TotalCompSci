#version 120

uniform mat4 P;
uniform mat4 MV;
uniform mat3 T;

attribute vec4 aPos;
attribute vec3 aNor;
varying vec4 aPosCam;
varying vec4 aNorCam;
attribute vec2 aTex;

varying vec2 vTex0;
varying vec3 vTex1;

uniform float t;

void main()
{
	gl_Position = P * MV * aPos;
	aPosCam = MV * aPos;
	aNorCam = MV * vec4(aNor.x, aNor.y, aNor.z, 0.0f);
	vTex0 = aTex;
	vTex1 = vec3(aTex.x, aTex.y, 1.0f) * (T); 
}
