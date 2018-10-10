#include "Material.h"
//Constructor
Material::Material(glm::vec3 _ka, glm::vec3 _kd, glm::vec3 _ks, float _s, bool _selected) {
	ka = _ka;
	kd = _kd;
	ks = _ks;
	s = _s;
	selected = _selected;
}
//Select functions
bool Material::is_selected() {
	return selected;
}

void Material::select() {
	selected = true;
}

void Material::unselect() {
	selected = false;
}
//Return functions
glm::vec3 Material::get_ka() {
	return ka;
}

glm::vec3 Material::get_kd() {
	return kd;
}

glm::vec3 Material::get_ks() {
	return ks;
}

float Material::get_s(){
	return s;
}
