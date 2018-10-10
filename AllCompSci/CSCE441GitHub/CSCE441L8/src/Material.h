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

class Material {
private:
	glm::vec3 ka;
	glm::vec3 kd;
	glm::vec3 ks;
	float s;
	bool selected;
public:
	Material(glm::vec3 _ka, glm::vec3 _kd, glm::vec3 _ks, float _s, bool _selected);
	bool is_selected();
	void select();
	void unselect();
	glm::vec3 get_ka();
	glm::vec3 get_kd();
	glm::vec3 get_ks();
	float get_s();
};