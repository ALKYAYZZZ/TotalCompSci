#include "Component.h"
//Default constructor
Component::Component() {

}

//Set which component is the root
Component::Component(bool is_roots) {
	root = is_roots;
}

//Return if the component is the root
bool Component::is_root() {
	return root;
}

//Return if the component is currently selected
bool Component::is_selected() {
	return selected;
}

//Sselect component on robot by increasing scale of x y and z values by
//a factor of .1 
void Component::select() {
	glm::vec3 temp = this->get_scaling_factors();
	temp.x = temp.x + .1f;
	temp.y = temp.y + .1f;
	temp.z = temp.z + .1f;
	this->set_scaling_factors(temp);
	selected = true;
}

//Unselect component on robot by decreasing scale of x y and z values by
//a factor of .1 
void Component::unselect() {
	glm::vec3 temp = this->get_scaling_factors();
	temp.x = temp.x - .1f;
	temp.y = temp.y - .1f;
	temp.z = temp.z - .1f;
	this->set_scaling_factors(temp);
	selected = false;
}

bool Component::has_children() {
	if (children.size() == 0) {
		return false;
	}
	else {
		return true;
	}
}

//Boolean return if component has a parent
bool Component::has_parent() {
	if (parent == NULL) {
		return false;
	}
	else {
		return true;
	}
}


//Get functions
Component* Component::get_parent() {
	return parent;
}

vector<Component*> Component::get_children() {
	return children;
}

glm::vec3 Component::get_translation_to_parent() {
	return translation_to_parent;
}

glm::vec3 Component::get_joint_angles() {
	return joint_angles;
}

glm::vec3 Component::get_translation_to_mesh() {
	return translation_to_mesh;
}

glm::vec3 Component::get_scaling_factors() {
	return scaling_factors;
}

glm::vec3 Component::get_joint_location() {
	return joint_location;
}

glm::vec3 Component::get_mesh_location() {
	return mesh_location;
}


//The following 6 functions increment and decrement the x y and z joint angles by a factor of .1 radians
void Component::increment_z() {
	glm::vec3 decrease_z_1 = this->get_joint_angles();
	decrease_z_1.z = decrease_z_1.z + .1f;
	this->set_joint_angles(decrease_z_1);
}
void Component::decrement_z() {
	glm::vec3 decrease_z_1 = this->get_joint_angles();
	decrease_z_1.z = decrease_z_1.z - .1f;
	this->set_joint_angles(decrease_z_1);
}
void Component::increment_y() {
	glm::vec3 decrease_y_1 = this->get_joint_angles();
	decrease_y_1.y = decrease_y_1.y + .1f;
	this->set_joint_angles(decrease_y_1);
}
void Component::decrement_y() {
	glm::vec3 decrease_y_1 = this->get_joint_angles();
	decrease_y_1.y = decrease_y_1.y -.1f;
	this->set_joint_angles(decrease_y_1);
}
void Component::increment_x() {
	glm::vec3 decrease_x_1 = this->get_joint_angles();
	decrease_x_1.x = decrease_x_1.x + .1f;
	this->set_joint_angles(decrease_x_1);
}
void Component::decrement_x() {
	glm::vec3 decrease_x_1 = this->get_joint_angles();
	decrease_x_1.x = decrease_x_1.x - .1f;
	this->set_joint_angles(decrease_x_1);
}
//Forward Depth first search to traverse forward all using recursion
Component* Component::move_selection_down(Component* previous, bool last) {
	Component* temp = new Component();
	if (last == false) {
		if (this->is_selected() == true) {
			if (this->has_children() == true) {
				for (size_t i = 0; i < this->get_children().size(); i++) {
					temp = this->get_children()[i]->move_selection_down(this, true);
					return temp;
				}
			}
			else {
				return this;
			}
		}
		else {
			if (this->has_children() == true) {
				for (size_t i = 0; i < this->get_children().size(); i++) {
					temp = this->get_children()[i]->move_selection_down(this, false);
					if (temp == NULL) {
						return NULL;
					}
					if (temp->is_selected() == true) {
						if ((i + 1) < this->get_children().size()) {
							temp = this->get_children()[i + 1]->move_selection_down(temp, true);
							return temp;
						}
						else {
							if (this->is_root()) {
								temp = this->move_selection_down(temp, true);
								return temp;
							}
							else {
								return temp;
							}
						}
					}
				}
			}
		}
	}
	else {
		previous->unselect();
		this->select();
		return NULL;
	}
	return this;
}

//Reverse Depth first search to traverse backwards all using recursion.
Component* Component::move_selection_up(Component* above, bool last) {
	
	Component* temp = new Component();
	if (last == false) {
		if (above->is_selected() == true) {
			if (this->has_children() == true) {
				for (int i = this->get_children().size() - 1; i >= 0; i--) {
					temp = this->get_children()[i]->move_selection_up(above, true);
					return temp;
				}
			}
			else {
				return this;
			}
		}
		else {
			if (this->has_children() == true) {
				for (int i = this->get_children().size() - 1; i >= 0; i--) {
					temp = this->get_children()[i]->move_selection_up(this, false);
					if (temp == NULL) {
						return NULL;
					}
					if (temp->is_selected() == true) {
						if ((i - 1) >= 0) {
							if (this->get_children()[i - 1]->has_children() == true && temp->has_children() == true) {
								temp = this->get_children()[i - 1]->move_selection_up(temp, false);
								return temp;
							}
							else {
								temp = this->get_children()[i - 1]->move_selection_up(temp, true);
								return temp;
							}
						}
						else {
							if (this->is_root()) {
								temp = this->move_selection_up(temp, true);
								return temp;
							}
							else {
								temp = this->move_selection_up(temp, true);
								return temp;
							}
						}
					}
				}
			}
		}
	}
	else {
		above->unselect();
		this->select();
		return NULL;
	}
	return this;
	
}



//Calculate Joint Location, type 1 is head, type 2 is left arm, type 3 is right arm,
//type 4 is left leg, type 5 is right leg.
glm::vec3 Component::calculate_joint_location(glm::vec3 this_mesh, int _type) {
	mesh_location = this_mesh;
	if (_type == 1) {
		double y = this_mesh.y - (.5 * this->get_scaling_factors().y);
		joint_location = glm::vec3(this_mesh.x, y, this_mesh.z);
		return glm::vec3(this_mesh.x, y, this_mesh.z);
		type = _type;
	}
	else if (_type == 2) {
		double x = this_mesh.x - (.5 * this->get_scaling_factors().x);
		joint_location = glm::vec3(x, this_mesh.y, this_mesh.z);
		return glm::vec3(x, this_mesh.y, this_mesh.z);
		type = _type;
	}
	else if (_type == 3) {
		double x = this_mesh.x + (.5 * this->get_scaling_factors().x);
		joint_location = glm::vec3(x, this_mesh.y, this_mesh.z);
		return glm::vec3(x, this_mesh.y, this_mesh.z);
		type = _type;
	}
	else if (_type == 4) {
		double y = this_mesh.y + (.5 * this->get_scaling_factors().y);
		joint_location = glm::vec3(this_mesh.x, y, this_mesh.z);
		return glm::vec3(this_mesh.x, y, this_mesh.z);
		type = _type;
	}
	else if (_type == 5) {
		double y = this_mesh.y + (.5 * this->get_scaling_factors().y);
		joint_location = glm::vec3(this_mesh.x, y, this_mesh.z);
		return glm::vec3(this_mesh.x, y, this_mesh.z);
		type = _type;
	}
	return glm::vec3(0, 0, 0);
}

//Set translation to parent by subtracting child and parent joint location
void Component::set_translation_to_parent(glm::vec3 input_child, glm::vec3 input_parent) {
	double x = input_child.x - input_parent.x;
	double y = input_child.y - input_parent.y;
	double z = input_child.z - input_parent.z;
	translation_to_parent = glm::vec3(x, y, z);
}

//Set joint angles
void Component::set_joint_angles(glm::vec3 input) {
	joint_angles = input;
}

//Set translation to mesh for torso
void Component::set_translation_to_mesh(glm::vec3 input) {
	translation_to_mesh = input;
}

//Set translation to mesh using mesh input and joint input vec3
void Component::set_translation_to_mesh(glm::vec3 input_mesh, glm::vec3 input_joint) {
	double x = input_mesh.x - input_joint.x;
	double y = input_mesh.y - input_joint.y;
	double z = input_mesh.z - input_joint.z;
	translation_to_mesh = glm::vec3(x, y, z);
}

//Set scaling factors
void Component::set_scaling_factors(glm::vec3 input) {
	scaling_factors = input;
}

//Add a parent to the component tree
void Component::add_parent(Component* new_parent) {
	parent = new_parent;
}

//Add a child to the component tree
void Component::add_child(Component* child) {
	child->add_parent(this);
	children.push_back(child);
}

//Draw the robot using recursion and updated angles
void Component::draw(shared_ptr<MatrixStack> &MV, shared_ptr<Program> &prog, shared_ptr<Shape> &shape, string RES_DIR) {
	MV->pushMatrix();
	MV->translate(this->get_translation_to_parent());
	MV->rotate(this->get_joint_angles().x, 1, 0, 0);
	MV->rotate(this->get_joint_angles().y, 0, 1, 0);
	MV->rotate(this->get_joint_angles().z, 0, 0, 1);
	MV->pushMatrix();
	MV->translate(this->get_translation_to_mesh());
	MV->scale(this->get_scaling_factors());
	glUniformMatrix4fv(prog->getUniform("MV"), 1, GL_FALSE, &MV->topMatrix()[0][0]);
	shape->draw(prog);
	MV->popMatrix();
	if (this->children.size() != 0) {
		for (size_t i = 0; i < this->children.size(); i++) {
			this->children[i]->draw(MV, prog, shape, RES_DIR);
		}
	}
	MV->popMatrix();
	
}