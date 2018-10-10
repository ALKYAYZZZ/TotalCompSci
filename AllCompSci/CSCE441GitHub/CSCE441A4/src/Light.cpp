#include "Light.h"

//Constructor
Light::Light(glm::vec4 _light_pos, bool _selected) {
	light_pos = _light_pos;
	selected = _selected;
}
//Return selected boolean
bool Light::is_selected() {
	return selected;
}
//Select light
void Light::select() {
	selected = true;
}
//Unselect light
void Light::unselect() {
	selected = false;
}
//Return light position
glm::vec4 Light::get_light_pos(){
	return light_pos;
}
//Add .1 to x
void Light::increment_x() {
	glm::vec4 temp = light_pos;
	temp.x = temp.x + .1f;
	light_pos = temp;
}
//Subtract .1 from x
void Light::decrement_x() {
	glm::vec4 temp = light_pos;
	temp.x = temp.x - .1f;
	light_pos = temp;
}
//Add .1 to y
void Light::increment_y() {
	glm::vec4 temp = light_pos;
	temp.y = temp.y + .1f;
	light_pos = temp;
}
//Subtract .1 from y
void Light::decrement_y() {
	glm::vec4 temp = light_pos;
	temp.y = temp.y - .1f;
	light_pos = temp;
}
