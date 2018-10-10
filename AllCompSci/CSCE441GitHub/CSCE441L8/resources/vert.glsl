#version 120

uniform mat4 P;
uniform mat4 MV;

attribute vec4 aPos; // in object space
attribute vec3 aNor; // in object space

varying vec4 aNorCam;
varying vec4 aPosCam;

void main()
{
	aNorCam = vec4(aNor.x, aNor.y, aNor.z, 0.0);
	aNorCam = MV * aNorCam;
	aPosCam = MV * aPos;
	
	gl_Position = P * MV * aPos;
	
}
