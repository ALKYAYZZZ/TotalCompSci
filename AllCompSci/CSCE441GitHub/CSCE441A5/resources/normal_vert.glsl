#version 120
attribute vec4 aPos;
attribute vec3 aNor;
uniform mat4 P;
uniform mat4 MV;
varying vec3 vNor;
varying vec4 aNorCam;
varying vec4 aPosCam;

void main()
{
	aNorCam = vec4(aNor.x, aNor.y, aNor.z, 0.0);
	aNorCam = MV * aNorCam;
	aPosCam = MV * aPos;
	
	gl_Position = P * MV * aPos;
}
