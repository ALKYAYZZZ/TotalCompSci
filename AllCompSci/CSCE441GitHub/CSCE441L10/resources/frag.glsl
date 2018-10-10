#version 120

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform vec3 lightPosCam;
varying vec4 aPosCam;
varying vec4 aNorCam;

varying vec2 vTex0;
varying vec3 vTex1;



void main()
{
	vec3 kd = texture2D(texture0, vTex0).rgb;
	vec3 ks = texture2D(texture1, vTex0).rgb;
	vec3 cloud = texture2D(texture2, vec2(vTex1.x, vTex1.y)).rgb;
	vec3 lightvec = lightPosCam - vec3(aPosCam);
	vec3 eyevec = vec3(0.0f,0.0f,0.0f) - vec3(aPosCam);
	vec3 halfway = normalize(lightvec) + normalize(eyevec);
	vec3 diff = kd * max(0,dot(normalize(lightvec),normalize(vec3(aNorCam))));
	vec3 spec = ks * pow(max(0,dot(normalize(halfway), normalize(vec3(aNorCam)))),50.0f);
	vec3 total = diff + spec + cloud;
	gl_FragColor = vec4(total.x, total.y, total.z, 1.0f);
}
