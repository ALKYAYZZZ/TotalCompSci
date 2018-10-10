#version 120

varying vec3 vNor;
uniform vec4 lightPos;
uniform vec3 ka;
uniform vec3 kd;
uniform vec3 ks;
uniform float s;
uniform float t;

varying vec4 aNorCam;
varying vec4 aPosCam;


void main()
{
	//Second light camera
	vec4 lightPosCam = vec4(lightPos.x, lightPos.y, lightPos.z, 0.0);
	vec4 lightvec = lightPosCam;
	vec3 eyevec = vec3(0,0,0) - vec3(aPosCam);
	eyevec = normalize(eyevec);
	vec3 lp = normalize(vec3(lightvec));
	vec3 halfway = (eyevec + lp);
	vec3 n = normalize(vec3(aNorCam));
	vec3 h = normalize(vec3(halfway));
	vec3 color = .5 * (n + 1.0);
	vec3 diff = kd * max(0, dot(lp, n));
	vec3 spec = ks * pow(max(0,dot(h,n)),s);
	vec3 final_color = lightPos.w * (ka + diff + spec);

	//Add 2 lights together
	gl_FragColor = vec4(final_color.r, final_color.g, final_color.b, 1.0);

	/*
	vec3 normal = normalize(vNor);
	// Map normal in the range [-1, 1] to color in range [0, 1];
	vec3 color = 0.5*normal + 0.5;
	gl_FragColor = vec4(color, 1.0);
	*/
}
