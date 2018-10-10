#version 120

attribute vec4 aPos;
attribute vec3 aNor;
attribute vec4 weights0;
attribute vec4 weights1;
attribute vec4 weights2;
attribute vec4 weights3;
attribute vec4 bones0;
attribute vec4 bones1;
attribute vec4 bones2;
attribute vec4 bones3;
attribute float numInfl;
uniform mat4 M[18];
uniform mat4 A[18];
uniform float g;

uniform mat4 P;
uniform mat4 MV;
varying vec3 vNor;
varying vec4 aNorCam;
varying vec4 aPosCam;
varying vec4 totalPos;
varying vec4 totalNorm;

void main()
{
	//If GPU skinning is selected, add up all bone weight calculations per vertex
	if(int(g) == 1){
		aNorCam = vec4(aNor.x, aNor.y, aNor.z, 0.0);
		totalPos = vec4(0.0,0.0,0.0,0.0);
		totalNorm = vec4(0.0,0.0,0.0,0.0);
		totalPos = totalPos + (weights0.w *(A[int(bones0.w)] *(M[int(bones0.w)] * aPos)));
		totalPos = totalPos + (weights0.x *(A[int(bones0.x)] *(M[int(bones0.x)] * aPos))); 
		totalPos = totalPos + (weights0.y *(A[int(bones0.y)] *(M[int(bones0.y)] * aPos))); 
		totalPos = totalPos + (weights0.z *(A[int(bones0.z)] *(M[int(bones0.z)] * aPos))); 
		totalPos = totalPos + (weights1.w *(A[int(bones1.w)] *(M[int(bones1.w)] * aPos)));
		totalPos = totalPos + (weights1.x *(A[int(bones1.x)] *(M[int(bones1.x)] * aPos))); 
		totalPos = totalPos + (weights1.y *(A[int(bones1.y)] *(M[int(bones1.y)] * aPos))); 
		totalPos = totalPos + (weights1.z *(A[int(bones1.z)] *(M[int(bones1.z)] * aPos)));
		totalPos = totalPos + (weights2.w *(A[int(bones2.w)] *(M[int(bones2.w)] * aPos)));
		totalPos = totalPos + (weights2.x *(A[int(bones2.x)] *(M[int(bones2.x)] * aPos))); 
		totalPos = totalPos + (weights2.y *(A[int(bones2.y)] *(M[int(bones2.y)] * aPos))); 
		totalPos = totalPos + (weights2.z *(A[int(bones2.z)] *(M[int(bones2.z)] * aPos)));
		totalPos = totalPos + (weights3.w *(A[int(bones3.w)] *(M[int(bones3.w)] * aPos)));
		totalPos = totalPos + (weights3.x *(A[int(bones3.x)] *(M[int(bones3.x)] * aPos))); 
		totalPos = totalPos + (weights3.y *(A[int(bones3.y)] *(M[int(bones3.y)] * aPos))); 
		totalPos = totalPos + (weights3.z *(A[int(bones3.z)] *(M[int(bones3.z)] * aPos)));
		totalNorm = totalNorm + (weights0.w *(A[int(bones0.w)] *(M[int(bones0.w)] * aNorCam)));
		totalNorm = totalNorm + (weights0.x *(A[int(bones0.x)] *(M[int(bones0.x)] * aNorCam))); 
		totalNorm = totalNorm + (weights0.y *(A[int(bones0.y)] *(M[int(bones0.y)] * aNorCam))); 
		totalNorm = totalNorm + (weights0.z *(A[int(bones0.z)] *(M[int(bones0.z)] * aNorCam))); 
		totalNorm = totalNorm + (weights1.w *(A[int(bones1.w)] *(M[int(bones1.w)] * aNorCam)));
		totalNorm = totalNorm + (weights1.x *(A[int(bones1.x)] *(M[int(bones1.x)] * aNorCam))); 
		totalNorm = totalNorm + (weights1.y *(A[int(bones1.y)] *(M[int(bones1.y)] * aNorCam))); 
		totalNorm = totalNorm + (weights1.z *(A[int(bones1.z)] *(M[int(bones1.z)] * aNorCam)));
		totalNorm = totalNorm + (weights2.w *(A[int(bones2.w)] *(M[int(bones2.w)] * aNorCam)));
		totalNorm = totalNorm + (weights2.x *(A[int(bones2.x)] *(M[int(bones2.x)] * aNorCam))); 
		totalNorm = totalNorm + (weights2.y *(A[int(bones2.y)] *(M[int(bones2.y)] * aNorCam))); 
		totalNorm = totalNorm + (weights2.z *(A[int(bones2.z)] *(M[int(bones2.z)] * aNorCam)));
		totalNorm = totalNorm + (weights3.w *(A[int(bones3.w)] *(M[int(bones3.w)] * aNorCam)));
		totalNorm = totalNorm + (weights3.x *(A[int(bones3.x)] *(M[int(bones3.x)] * aNorCam))); 
		totalNorm = totalNorm + (weights3.y *(A[int(bones3.y)] *(M[int(bones3.y)] * aNorCam))); 
		totalNorm = totalNorm + (weights3.z *(A[int(bones3.z)] *(M[int(bones3.z)] * aNorCam)));      
		aNorCam = MV * totalNorm;
		aPosCam = MV * totalPos;
		gl_Position = P * (MV * totalPos);
	}
	//For CPU skinning, use normal position and normal calculations
	else{
		aNorCam = vec4(aNor.x, aNor.y, aNor.z, 0.0);
		aNorCam = MV * aNorCam;
		aPosCam = MV * aPos;
	
		gl_Position = P * MV * aPos;
	
	}

}
