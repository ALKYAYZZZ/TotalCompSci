#include "Object.h"

Object::Object(int _desc) {
	desc = _desc;
}

//Randomly Distributed the objects on the same y plane in between -100 and 100 x and z.
//The random rotation about the y axis is also calculated.
void Object::chooseCoords() {
	x = (float)(rand() % 101 - 50);
	y = 0.5f;
	z = (float)(rand() % 101 - 50);
	rotz = (float)rand();
}

//Return x Value
float Object::getX() {
	return x;
}

//Return y Value
float Object::getY() {
	return y;
}

//Return Z Value
float Object::getZ() {
	return z;
}

//Return Rotation Value
float Object::getRotZ() {
	return rotz;
}

//Return Descriptor Discerning between which shape will be drawn.
int Object::getDesc() {
	return desc;
}
//Randomly Decides the Color of the Object
void Object::setDiffComp() {
	diffcomp = glm::vec3(static_cast <float> (rand()) / static_cast <float> (RAND_MAX), static_cast <float> (rand()) / static_cast <float> (RAND_MAX), static_cast <float> (rand()) / static_cast <float> (RAND_MAX));
}
//Returns Color of Object
glm::vec3 Object::getDiffComp() {
	return diffcomp;
}
