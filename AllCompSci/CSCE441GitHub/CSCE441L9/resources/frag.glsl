#version 120

uniform sampler2D texture0;
uniform sampler2D texture1;

varying vec2 vTex0;
varying vec3 vTex1;

void main()
{
	vec4 color0 = texture2D(texture0, vTex0);
	vec4 color1 = texture2D(texture1, vec2(vTex1.x,vTex1.y));
	if(color1.x >= .95f && color1.y >= .95f && color1.z >= .95f){
		color1 = color0;
	}
	gl_FragColor = color1;
}
