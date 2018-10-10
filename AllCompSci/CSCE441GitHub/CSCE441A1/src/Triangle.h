#ifndef TRI_HEAD
#define TRI_HEAD
#include <string>
#include <vector>
#include <iostream>
#include <vector>
#include <memory>
#include "Image.h"

using namespace std;

class Triangle {
public:
	Triangle(float vx1, float vx2, float vx3, float vy1,float vy2, float vy3, float vz1, float vz2, float vz3);
	//Used to initialize temporary triangle
	Triangle() : Triangle(0, 0, 0, 0, 0, 0, 0, 0, 0) {};
	float x1;
	float x2;
	float x3;
	float y1;
	float y2;
	float y3;
	float z1;
	float z2;
	float z3;
	float find_highest(float a, float b, float c);
	float find_lowest(float a, float b, float c);

};

#endif
