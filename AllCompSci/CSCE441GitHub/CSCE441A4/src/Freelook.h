#pragma  once
#ifndef __Freelook__
#define __Freelook__

#include <memory>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>


class MatrixStack;

class Freelook
{
public:
	enum {
		ROTATE = 0,
		TRANSLATE,
		SCALE
	};
	
	Freelook();
	virtual ~Freelook();
	void setInitDistance(float z) { translations.z = -std::abs(z); }
	void setAspect(float a) { aspect = a; };
	void setRotationFactor(float f) { rfactor = f; };
	void setTranslationFactor(float f) { tfactor = f; };
	void setScaleFactor(float f) { sfactor = f; };
	void mouseClicked(float x, float y, bool shift, bool ctrl, bool alt);
	//This Function below was updated from the original to take in the monitor variable to
	//Use the height and width components.
	void mouseMoved(float x, float y, GLFWwindow *monitor);
	void applyProjectionMatrix(std::shared_ptr<MatrixStack> P) const;
	void applyViewMatrix(std::shared_ptr<MatrixStack> MV) const;
	//Movement Functions
	void moveW();
	void moveA();
	void moveS();
	void moveD();
	//Return Functions
	glm::vec3 getYaw();
	glm::vec3 getPitch();
	//Yaw and Pitch Function
	void updateYaw(float diff, float pitch);
	
private:
	float aspect;
	float fovy;
	float znear;
	float zfar;
	glm::vec2 rotations;
	glm::vec3 translations;
	glm::vec2 mousePrev;
	int state;
	float rfactor;
	float tfactor;
	float sfactor;
	float yaw = 0;
	float pitch = 0;
	//Yaw and Pitch Matricies
	glm::vec3 yawmat;
	glm::vec3 pitchmat;
};

#endif
