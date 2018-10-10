#version 120

uniform mat4 P;
uniform mat4 MV;
uniform float t;
attribute vec4 aPos; // In object space
attribute vec3 aNor; // In object space
varying vec3 color;

void main()
{
	float temp = (1/20.0f) * (cos((100.0f * aPos.x) + t) + sin((100.0f * aPos.y) + t));
	vec4 newPos = vec4(aPos.x, aPos.y, temp, aPos.w);
	gl_Position = P * MV * newPos;
	vec4 j = normalize(MV * vec4(aNor, 0.0));
	vec3 matrix = cross(vec3(1,0, -sin((100.0f * aPos.x) + t)), vec3(0, 1, cos((100.0f * aPos.y) + t)));
	vec4 n = normalize(MV * vec4(matrix, 0.0));
	vec3 l = normalize(vec3(0.0, 0.0, 1.0)); // In camera space
	color = vec3(1.0, 1.0, 1.0) * max(0,dot(l,vec3(n))) + (.000000001f * aNor);
}
