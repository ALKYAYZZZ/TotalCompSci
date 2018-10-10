#version 120

attribute vec4 aPos;
attribute vec2 aTex;
attribute vec2 posLocal;
uniform vec2 cps[25];
attribute float tileIndex;
uniform mat4 P;
uniform mat4 MV;
uniform float g;
varying vec2 vTex;
varying vec4 aPosnew;

void main()
{
	if(int(g) != 1){
		gl_Position = P * MV * aPos;
		vTex = aTex;
	}
	else{
		int ncols = 5;
		int tile = int(tileIndex);
		vec2 cp00 = cps[tile];
		vec2 cp01 = cps[tile + 1];
		vec2 cp10 = cps[tile + ncols];
		vec2 cp11 = cps[tile + ncols + 1];
		vec2 p0 = ((1 - posLocal.x) * cp00) + (posLocal.x * cp01);
		vec2 p1 = ((1 - posLocal.x) * cp10) + (posLocal.x * cp11);
		vec2 p = ((1 - posLocal.y) * p0) + (posLocal.y * p1);
		vec3 aPostemp = vec3(aPos);
		aPosnew = vec4(p.x, p.y, aPostemp.z, 1.0f);
		gl_Position = P * (MV * aPosnew);
		vTex = aTex;
	}
}
