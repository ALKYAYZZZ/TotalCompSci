#include "Triangle.h"


//Initialize Triangle with all 9 given coordinates
Triangle::Triangle(float vx1, float vx2, float vx3, float vy1, float vy2, float vy3, float vz1, float vz2, float vz3) {
	x1 = vx1;
	x2 = vx2;
	x3 = vx3;
	y1 = vy1;
	y2 = vy2;
	y3 = vy3;
	z1 = vz1;
	z2 = vz2;
	z3 = vz3;
}

//Simple formula to find highest value of three given coordinates
float Triangle::find_highest(float a, float b, float c) {
	if (a > b) {
		if (a > c) {
			return a;
		}
		else {
			return c;
		}
	}
	else {
		if (b > c) {
			return b;
		}
		else {
			return c;
		}
	}
}

//Simple formula to determine lowest value out of three given coordinates
float Triangle::find_lowest(float a, float b, float c) {
	if (a < b) {
		if (a < c) {
			return a;
		}
		else {
			return c;
		}
	}
	else {
		if (b < c) {
			return b;
		}
		else {
			return c;
		}
	}
}

