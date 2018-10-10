#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <vector>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "GLSL.h"
#include "MatrixStack.h"
#include "Program.h"
#include "Shape.h"

using namespace std;

class Component {
	//Private member variables
	Component* parent;
	vector<Component*> children;
	bool root;
	bool selected;
	int type;
	glm::vec3 translation_to_parent;
	glm::vec3 joint_angles;
	glm::vec3 translation_to_mesh;
	glm::vec3 scaling_factors;
	glm::vec3 joint_location;
	glm::vec3 mesh_location;

public:
	//Constructor
	Component();
	Component(bool is_roots);

	//Get Functions
	bool is_root();
	bool has_children();
	bool has_parent();
	bool is_selected();
	void select();
	void unselect();
	Component* get_parent();
	vector<Component*> get_children();
	glm::vec3 get_translation_to_parent();
	glm::vec3 get_joint_angles();
	glm::vec3 get_translation_to_mesh();
	glm::vec3 get_scaling_factors();
	glm::vec3 calculate_joint_location(glm::vec3 this_mesh, int type);
	glm::vec3 get_joint_location();
	glm::vec3 get_mesh_location();
	//Set functions
	void set_translation_to_parent(glm::vec3 input_child, glm::vec3 input_parent);
	void set_joint_angles(glm::vec3 input);
	void set_translation_to_mesh(glm::vec3 input);
	void set_translation_to_mesh(glm::vec3 input_mesh, glm::vec3 input_joint);
	void set_scaling_factors(glm::vec3 input);

	//Traversal functions
	Component* move_selection_up(Component* previous, bool last);
	Component* move_selection_down(Component* previous, bool last);

	//Scaling factor increase and decrease functions
	void increment_z();
	void decrement_z();
	void increment_y();
	void decrement_y();
	void increment_x();
	void decrement_x();


	//Tree Functions
	void add_parent(Component* parent);
	void add_child(Component* child);
	
	//Other Functions
	void draw(shared_ptr<MatrixStack> &MV, shared_ptr<Program> &prog, shared_ptr<Shape> &shape, string RES_DIR);



};