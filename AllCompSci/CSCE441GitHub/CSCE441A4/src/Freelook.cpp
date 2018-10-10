#define _USE_MATH_DEFINES
#include <cmath> 
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include "Freelook.h"
#include "MatrixStack.h"

Freelook::Freelook() :
	aspect(1.0f),
	fovy((float)(45.0*M_PI/180.0)),
	znear(0.1f),
	zfar(1000.0f),
	rotations(0.0, 0.0),
	translations(0.0f, 0.0f, -5.0f),
	rfactor(0.01f),
	tfactor(0.001f),
	sfactor(0.005f),
	yawmat(glm::vec3(sin(0), 0, cos(0))),
	pitchmat(glm::vec3(sin(0), 0, cos(0)))
{
}

Freelook::~Freelook()
{
}

//Function Implementing What Happens When The mouse is clicked
void Freelook::mouseClicked(float x, float y, bool shift, bool ctrl, bool alt)
{
	mousePrev.x = x;
	mousePrev.y = y;
	if(shift) {
		state = Freelook::TRANSLATE;
	} else if(ctrl) {
		state = Freelook::SCALE;
	} else {
		state = Freelook::ROTATE;
	}
}

//Function That Calculates the Pitch and Yaw Angle by Taking the Tangent of zfar and the x/y movement of the mouse.
void Freelook::mouseMoved(float x, float y, GLFWwindow *monitor)
{
	glm::vec2 mouseCurr(x, y);
	glm::vec2 dv = mouseCurr - mousePrev;
	float temp = translations.z;
	switch(state) {
		case Freelook::ROTATE:
			rotations += rfactor * dv;
			break;
		case Freelook::TRANSLATE:
			translations.x -= translations.z * tfactor * dv.x;
			translations.y += translations.z * tfactor * dv.y;
			break;
		case Freelook::SCALE:
			translations.z *= (1.0f - sfactor * dv.y);
			break;
	}
	//Get window width and height to center mouse movement
	int width, height;
	glfwGetWindowSize(monitor, &width, &height);

	//Update the Yaw when the mouse is moved.
	yaw = yaw - (atan2(mouseCurr.x - (float)((float)width / 2.0f) , zfar) * .1f);
	//pitch = pitch + atan2(mouseCurr.y - (float)((float)height / 2.0f), zfar);
	this->updateYaw(yaw, - (atan2(mouseCurr.y - (float)((float)height / 2.0f) , zfar) * 0.75f));
	mousePrev = mouseCurr;

	
}

void Freelook::applyProjectionMatrix(std::shared_ptr<MatrixStack> P) const
{
	// Modify provided MatrixStack
	P->multMatrix(glm::perspective(fovy, aspect, znear, zfar));
}

//Multiply the Model View Matrix by the View Matrix, using the lookAt function.
void Freelook::applyViewMatrix(std::shared_ptr<MatrixStack> MV) const
{
	MV->multMatrix((glm::lookAt(translations, translations + yawmat, glm::vec3(0.0f, 1.0f, 0.0f))));
	
}

//Move in the forward direction, calculated by adding the Yaw components calculated above.
void Freelook::moveW() {
	glm::vec3 temp = translations;
	glm::vec3 temp2 = yawmat;
	translations = glm::vec3(temp.x + temp2.x * 0.5f, temp.y * 0.5f, temp.z + temp2.z * 0.5f);
}

//Move in the Back direction, calculated by subtracting the Yaw components calculated above.
void Freelook::moveS() {
	glm::vec3 temp = translations;
	glm::vec3 temp2 = yawmat;
	translations = glm::vec3(temp.x - temp2.x * 0.5f, temp.y * 0.5f, temp.z - temp2.z * 0.5f);
}

//Move in the left direction, calculated by cross multiplying the Up vector with the 
//Yaw vector and adding the solution to the translation matrix.
void Freelook::moveA() {
	glm::vec3 temp = translations;
	glm::vec3 temp2 = yawmat;
	glm::vec3 right = glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), yawmat);
	translations =  glm::vec3(temp.x + right.x * 0.5f, temp.y * 0.5f, temp.z + right.z * 0.5f);
}

//Move in the right direction, calculated by cross multiplying the Up vector with the 
//Yaw vector and subtracting the solution to the translation matrix.
void Freelook::moveD() {
	glm::vec3 temp = translations;
	glm::vec3 temp2 = yawmat;
	glm::vec3 right = glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), yawmat);
	translations = glm::vec3(temp.x - right.x * 0.5f, temp.y * 0.5f, temp.z - right.z * 0.5f);
}

//Return Yaw Matrix
glm::vec3 Freelook::getYaw() {
	return yawmat;
}

//Update the Yaw Matrix when the mouse is moved. If the pitch is greater than 60 degrees or less than
//-60 degrees, cap it off so it doesnt move anymore. The pitch has the cos of the angle in the x and z
//matrix positions and sin of the angle in the y position. The yaw is the sin of the angle in the x component
//and cos in the z component. The final vector is (Yaw.x * Pitch.x,Pitch.y, Yaw.z * Pitch.z).
void Freelook::updateYaw(float diff, float pitch) {
	glm::vec3 temp = yawmat;
	glm::vec3 temp2 = pitchmat;
	if (pitch > 1.0472) {
		pitchmat = glm::vec3(cos(1.0472), sin(1.0472), cos(1.0472));
	}
	else if (pitch < -1.0472) {
		pitchmat = glm::vec3(cos(-1.0472), sin(-1.0472), cos(-1.0472));
	}
	else {
		pitchmat = glm::vec3(cos(pitch), sin(pitch), cos(pitch));
	}
	yawmat = glm::vec3(sin(diff) * pitchmat.x, pitchmat.y, cos(diff) * pitchmat.z );
}

//Return Pitch Matrix
glm::vec3 Freelook::getPitch() {
	return pitchmat;
}


