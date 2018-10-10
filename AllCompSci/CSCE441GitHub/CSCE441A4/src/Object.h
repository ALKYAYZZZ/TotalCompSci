#pragma once
#include <cassert>
#include <cstring>
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Object {
private:
	//Variables containing translation and Color Data
	float x;
	float y;
	float z;
	float rotz;
	int desc;
	glm::vec3 diffcomp;
public:
	//Constructor accepting a descriptor that decides which shape will be drawn.
	Object(int _desc);
	//Randomly Decide where the object will be placed.
	void chooseCoords();
	//Return Functions
	float getX();
	float getY();
	float getZ();
	float getRotZ();
	int getDesc();
	//Set Random Color For Object
	void setDiffComp();
	glm::vec3 getDiffComp();
};