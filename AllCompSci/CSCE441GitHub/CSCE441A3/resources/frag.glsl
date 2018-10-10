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
	//First light camera
	vec4 lightPosCam1 = vec4(lightPos1.x, lightPos1.y, lightPos1.z, 0.0);
	vec4 lightvec1 = lightPosCam1 - aPosCam;
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

	//Second light camera
	vec4 lightPosCam2 = vec4(lightPos2.x, lightPos2.y, lightPos2.z, 0.0);
	vec4 lightvec2 = lightPosCam2 - aPosCam;
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
	//Add 2 lights together
	gl_FragColor = vec4(final_color1.r + final_color2.r, final_color1.g + final_color2.g, final_color1.b + final_color2.b, 1.0);
}
