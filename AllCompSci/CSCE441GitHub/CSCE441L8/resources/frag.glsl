#version 120
uniform mat4 P;
uniform mat4 MV;


uniform vec3 lightPos;
uniform vec3 ka;
uniform vec3 kd;
uniform vec3 ks;
uniform float s;

varying vec4 aNorCam;
varying vec4 aPosCam;

void main()
{
	vec4 lightPosCam = vec4(lightPos.x, lightPos.y, lightPos.z, 0.0);
	vec4 lightvec = lightPosCam - aPosCam;
	vec3 eyevec = vec3(0,0,0) - vec3(aPosCam);
	eyevec = normalize(eyevec);
	vec3 lp = normalize(vec3(lightvec));
	vec3 halfway = (eyevec + lp);
	vec3 n = normalize(vec3(aNorCam));
	vec3 h = normalize(vec3(halfway));
	vec3 color = .5 * (n + 1.0);
	vec3 diff = kd * max(0, dot(lp, n));
	vec3 spec = ks * pow(max(0,dot(h,n)),s);
	vec3 final_color = ka + diff + spec;
	gl_FragColor = vec4(final_color.r, final_color.g, final_color.b, 1.0);
}
