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

class Light {
private:
	//Light Position vector
	glm::vec4 light_pos;
	//Selected or not
	bool selected;
public:
	//Constructor
	Light(glm::vec4 _light_pos, bool _selected);
	//Select functions
	bool is_selected();
	void select();
	void unselect();
	//Move functions
	void increment_x();
	void decrement_x();
	void increment_y();
	void decrement_y();
	//Return functions
	glm::vec4 get_light_pos();
};