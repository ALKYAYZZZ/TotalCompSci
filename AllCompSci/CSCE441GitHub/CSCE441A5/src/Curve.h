#pragma once
#include <cassert>
#include <cstring>
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <vector>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Curve {
private:
	//Private variables
	glm::mat4 Bcr, Bb;
	std::vector<std::pair<float, float> > usTable;
	glm::mat4 main_matrix;
	float smax = 0;
	float umax = 0;
	float tmax = 5.0f;
public:
	//Constructor
	Curve();
	//Select functions
	void build_curve(std::vector<glm::vec3> cps);
	void buildTable(std::vector<glm::vec3> cps, std::vector<glm::quat> qpos);
	float t2s(float t);
	float s2u(float s);
	//Return functions
	float get_smax();
	float get_umarker();
	glm::mat4 get_mainmatrix(std::vector<glm::vec3> cps, std::vector<glm::quat> qpos, float u);
	void inc_tmax();
	void dec_tmax();
};