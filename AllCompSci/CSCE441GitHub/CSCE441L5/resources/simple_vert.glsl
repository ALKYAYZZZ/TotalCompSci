#version 120

uniform mat4 P;
attribute vec3 vertPos;
attribute vec3 vertCol;
varying vec3 vMyColor;

void main()
{
	gl_Position = P * vec4(vertPos, 1.0);
	vMyColor =  vertCol;
}
