#version 120
uniform mat4 P;
uniform mat4 MV;
uniform mat4 V;
uniform sampler2D texture0;

uniform vec4 lightPos1;
uniform vec4 lightPos2;
uniform vec3 ka;
uniform vec3 kd;
uniform vec3 ks;
uniform float s;
uniform float t;

varying vec4 aNorCam;
varying vec4 aPosCam;
varying vec2 vTex0;

void main()
{
	//First light camera
	vec4 lightPosCam1 = vec4(lightPos1.x - (60 * sin(t/2)), lightPos1.y, lightPos1.z - (60 * cos(t/2)), 1.0);
	vec3 lightvec1 = vec3(V * lightPosCam1) - vec3(aPosCam);
	vec3 eyevec1 = vec3(0,0,0) - vec3(aPosCam);
	eyevec1 = normalize(eyevec1);
	vec3 lp1 = normalize(vec3(lightvec1));
	vec3 halfway1 = (eyevec1 + lp1);
	vec3 n1 = normalize(vec3(aNorCam));
	vec3 h1 = normalize(vec3(halfway1));
	vec3 color1 = .5 * (n1 + 1.0);
	vec3 diff1 = kd * max(0, dot(lp1, n1));
	vec3 spec1 = ks * pow(max(0,dot(h1,n1)),s);
	vec3 final_color1 = lightPos1.w * (ka + diff1 + spec1);

	/*
	//Second light camera
	vec4 lightPosCam2 = vec4(lightPos2.x, lightPos2.y, lightPos2.z, 0.0);
	vec4 lightvec2 = lightPosCam2;
	vec3 eyevec2 = vec3(0,0,0) - vec3(aPosCam);
	eyevec2 = normalize(eyevec2);
	vec3 lp2 = normalize(vec3(lightvec2));
	vec3 halfway2 = (eyevec2 + lp2);
	vec3 n2 = normalize(vec3(aNorCam));
	vec3 h2 = normalize(vec3(halfway2));
	vec3 color2 = .5 * (n2 + 1.0);
	vec3 diff2 = kd * max(0, dot(lp2, n2));
	vec3 spec2 = ks * pow(max(0,dot(h2,n2)),s);
	vec3 final_color2 = lightPos2.w * (ka + diff2 + spec2);
	*/

	vec4 color0 = texture2D(texture0, vTex0);

	//Add 2 lights together
	gl_FragColor = vec4(final_color1.r + color0.r, final_color1.g + color0.g, final_color1.b + color0.b, 1.0);
}
