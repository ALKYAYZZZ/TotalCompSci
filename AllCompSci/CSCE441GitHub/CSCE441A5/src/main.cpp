#include <iostream>
#include <vector>
#include <cmath>
#define _USE_MATH_DEFINES
#include <math.h>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Camera.h"
#include "GLSL.h"
#include "Program.h"
#include "MatrixStack.h"
#include "Shape.h"
#include "Material.h"
#include "Curve.h"

using namespace std;

bool keyToggles[256] = {false}; // only for English keyboards!

GLFWwindow *window; // Main application window
string RESOURCE_DIR = ""; // Where the resources are loaded from

shared_ptr<Program> progNormal;
shared_ptr<Program> progSimple;
shared_ptr<Camera> camera;
shared_ptr<Shape> hbody1;
shared_ptr<Shape> hbody2;
shared_ptr<Shape> hprop1;
shared_ptr<Shape> hprop2;
Curve * catmull = new Curve();
Material hbody2color = Material(glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(1.0f, 0.9f, 0.8f), 200.0f, true);
Material hbody1color = Material(glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.9f, 0.8f), 200.0f, true);
Material hpropcolor = Material(glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.7f, 0.7f, 0.7f), glm::vec3(1.0f, 0.9f, 0.8f), 200.0f, true);
vector<glm::vec3> cps;
vector<glm::quat> qpos;
glm::vec3 axis0, axis1, axis2, axis3, axis4, axis5, axis6, axis7, axis8;
glm::mat4 heli_matrix;


static void error_callback(int error, const char *description)
{
	cerr << description << endl;
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}

static void char_callback(GLFWwindow *window, unsigned int key)
{
	keyToggles[key] = !keyToggles[key];
	if (key == 'T') {
		catmull->inc_tmax();
	}


	if (key == 't') {
		catmull->dec_tmax();
	}
}

static void cursor_position_callback(GLFWwindow* window, double xmouse, double ymouse)
{
	int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	if(state == GLFW_PRESS) {
		camera->mouseMoved((float)xmouse, (float)ymouse);
	}
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	// Get the current mouse position.
	double xmouse, ymouse;
	glfwGetCursorPos(window, &xmouse, &ymouse);
	// Get current window size.
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	if(action == GLFW_PRESS) {
		bool shift = mods & GLFW_MOD_SHIFT;
		bool ctrl = mods & GLFW_MOD_SHIFT;
		bool alt = mods & GLFW_MOD_SHIFT;
		camera->mouseClicked((float)xmouse, (float)ymouse, shift, ctrl, alt);
	}
}

static void init()
{
	GLSL::checkVersion();
	
	// Set background color
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	// Enable z-buffer test
	glEnable(GL_DEPTH_TEST);
	
	keyToggles[(unsigned)'c'] = true;
	
	// For drawing the helicopter
	progNormal = make_shared<Program>();
	progNormal->setShaderNames(RESOURCE_DIR + "normal_vert.glsl", RESOURCE_DIR + "normal_frag.glsl");
	progNormal->setVerbose(true);
	progNormal->init();
	progNormal->addUniform("P");
	progNormal->addUniform("MV");
	progNormal->addAttribute("aPos");
	progNormal->addAttribute("aNor");
	progNormal->addUniform("lightPos");
	progNormal->addUniform("ka");
	progNormal->addUniform("kd");
	progNormal->addUniform("ks");
	progNormal->addUniform("s");
	progNormal->setVerbose(false);
	
	// For drawing the frames
	progSimple = make_shared<Program>();
	progSimple->setShaderNames(RESOURCE_DIR + "simple_vert.glsl", RESOURCE_DIR + "simple_frag.glsl");
	progSimple->setVerbose(true);
	progSimple->init();
	progSimple->addUniform("P");
	progSimple->addUniform("MV");
	progSimple->setVerbose(false);
	

	//Load all parts of helicopter
	hbody1 = make_shared<Shape>();
	hbody1->loadMesh(RESOURCE_DIR + "helicopter_body1.obj");
	hbody1->init();

	hbody2 = make_shared<Shape>();
	hbody2->loadMesh(RESOURCE_DIR + "helicopter_body2.obj");
	hbody2->init();

	hprop1 = make_shared<Shape>();
	hprop1->loadMesh(RESOURCE_DIR + "helicopter_prop1.obj");
	hprop1->init();

	hprop2 = make_shared<Shape>();
	hprop2->loadMesh(RESOURCE_DIR + "helicopter_prop2.obj");
	hprop2->init();
	
	camera = make_shared<Camera>();

	//Initialize rotation axis'
	axis0 = glm::vec3(0.0f,0.0f,0.0f);
	axis1 = glm::vec3(0.0f, 0.0f, 0.0f);
	axis2 = glm::vec3(rand(), rand(), rand());
	axis3 = glm::vec3(rand(), rand(), rand());
	axis4 = glm::vec3(rand(), rand(), rand());
	axis5 = glm::vec3(rand(), rand(), rand());
	axis6 = glm::vec3(rand(), rand(), rand());
	axis7 = glm::vec3(0.0f, 0.0f, 0.0f);
	axis8 = glm::vec3(0.0f, 0.0f, 0.0f);

	//Initialize Positions of Keyframes
	glm::vec3 p0(0.0f, 0.0f, 0.0f);
	glm::vec3 p1(0.0f, 0.0f, 0.0f);
	glm::vec3 p2(1.0f, 1.0f, 0.0f);
	glm::vec3 p3(2.0f, 3.0f, -0.5f);
	glm::vec3 p4(-100.0f, 5.0f, -1.0f);
	glm::vec3 p5(-3.0f, 1.0f, -3.0f);
	glm::vec3 p6(-1.0f, 0.5f, -1.0f);
	glm::vec3 p7(0.0f, 0.0f, 0.0f);
	glm::vec3 p8(0.0f, 0.0f, 0.0f);
	cps.push_back(p0);
	cps.push_back(p1);
	cps.push_back(p2);
	cps.push_back(p3);
	cps.push_back(p4);
	cps.push_back(p5);
	cps.push_back(p6);
	cps.push_back(p7);
	cps.push_back(p8);

	//Initialize quats
	glm::quat q0, q1, q2, q3, q4, q5, q6, q7, q8;
	if (glm::length(axis0) == 0) {
		q0 = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	}
	else {
		axis0 = glm::normalize(axis0);
		q0 = glm::angleAxis((float)(90.0f / 180.0f*M_PI), axis0);
	}
	if (glm::length(axis1) == 0) {
		q1 = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	}
	else {
		axis1 = glm::normalize(axis1);
		q1 = glm::angleAxis((float)(90.0f / 180.0f*M_PI), axis1);
	}
	if (glm::length(axis2) == 0) {
		q2 = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	}
	else {
		axis2 = glm::normalize(axis2);
		q2 = glm::angleAxis((float)(90.0f / 180.0f*M_PI), axis2);
	}
	if (glm::length(axis3) == 0) {
		q3 = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	}
	else {
		axis3 = glm::normalize(axis3);
		q3 = glm::angleAxis((float)(90.0f / 180.0f*M_PI), axis3);
	}
	if (glm::length(axis4) == 0) {
		q4 = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	}
	else {
		axis4 = glm::normalize(axis4);
		q4 = glm::angleAxis((float)(90.0f / 180.0f*M_PI), axis4);
	}
	if (glm::length(axis5) == 0) {
		q5 = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	}
	else {
		axis5 = glm::normalize(axis5);
		q5 = glm::angleAxis((float)(90.0f / 180.0f*M_PI), axis5);
	}
	if (glm::length(axis6) == 0) {
		q6 = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	}
	else {
		axis6 = glm::normalize(axis6);
		q6 = glm::angleAxis((float)(90.0f / 180.0f*M_PI), axis6);
	}
	if (glm::length(axis7) == 0) {
		q7 = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	}
	else {
		axis7 = glm::normalize(axis7);
		q7 = glm::angleAxis((float)(90.0f / 180.0f*M_PI), axis7);
	}
	if (glm::length(axis8) == 0) {
		q8 = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	}
	else {
		axis8 = glm::normalize(axis8);
		q8 = glm::angleAxis((float)(90.0f / 180.0f*M_PI), axis8);
	}
	qpos.push_back(q0);
	qpos.push_back(q1);
	qpos.push_back(q2);
	qpos.push_back(q3);
	qpos.push_back(q4);
	qpos.push_back(q5);
	qpos.push_back(q6);
	qpos.push_back(q7);
	qpos.push_back(q8);

	heli_matrix = glm::mat4();
	
	// Initialize time.
	glfwSetTime(0.0);
	
	// If there were any OpenGL errors, this will print something.
	// You can intersperse this line in your code to find the exact location
	// of your OpenGL error.
	GLSL::checkError(GET_FILE_LINE);
}

void render()
{
	// Update time.
	double t = glfwGetTime();
	
	// Get current frame buffer size.
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
	
	// Use the window size for camera.
	glfwGetWindowSize(window, &width, &height);
	camera->setAspect((float)width/(float)height);
	
	// Clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if(keyToggles[(unsigned)'c']) {
		glEnable(GL_CULL_FACE);
	} else {
		glDisable(GL_CULL_FACE);
	}
	if(keyToggles[(unsigned)'l']) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	} else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	
	auto P = make_shared<MatrixStack>();
	auto MV = make_shared<MatrixStack>();

	//Positions pushed onto main vector
	glm::vec3 p0 = cps[0];
	glm::vec3 p1 = cps[1];
	glm::vec3 p2 = cps[2];
	glm::vec3 p3 = cps[3];
	glm::vec3 p4 = cps[4];
	glm::vec3 p5 = cps[5];
	glm::vec3 p6 = cps[6];
	glm::vec3 p7 = cps[7];
	glm::vec3 p8 = cps[8];

	//Quats pushed onto main vector
	glm::quat q0 = qpos[0];
	glm::quat q1 = qpos[1];
	glm::quat q2 = qpos[2];
	glm::quat q3 = qpos[3];
	glm::quat q4 = qpos[4];
	glm::quat q5 = qpos[5];
	glm::quat q6 = qpos[6];
	glm::quat q7 = qpos[7];
	glm::quat q8 = qpos[8];
	
	
	// Apply camera transforms
	
		P->pushMatrix();
		camera->applyProjectionMatrix(P);
		MV->pushMatrix();
		if (!keyToggles[(unsigned)GLFW_KEY_SPACE]) {
			camera->applyViewMatrix(MV);
		}
		
		//If space is pressed, switch to helipcopter view mode
		if (keyToggles[(unsigned)GLFW_KEY_SPACE]) {
			camera->applyFirstPersonMatrix(MV, heli_matrix, 0, 0, -4.0f);
		}
			

	// Draw origin frame
		progSimple->bind();

		catmull->build_curve(cps);
		catmull->buildTable(cps, qpos);
		float s_temp = catmull->t2s((float)t);
		float u = catmull->s2u(s_temp);
		float u_marker = catmull->get_umarker() / 6.0f;
		progSimple->unbind();
	GLSL::checkError(GET_FILE_LINE);


	
	// Draw the bunnies
	progNormal->bind();

	// Send projection matrix (same for all heli)
	glUniformMatrix4fv(progNormal->getUniform("P"), 1, GL_FALSE, glm::value_ptr(P->topMatrix()));

	//Rotation Centers
	glm::vec3 prop1center(0.0, 0.4819, 0.0);
	glm::vec3 prop2center(0.6228, 0.1179, 0.1365);
	

	//Draw Catmull Rom Curve
	

	glm::mat4 R;

	glm::vec3 position;

	//Push Matrix for Interpolated Helicopter and Rotating Blades
	MV->pushMatrix();
	if (u <= 1) {
		R = glm::mat4_cast(glm::normalize((1.0f - u) * q1 + u * q2));
		MV->translate(((1 - u) * p1) + ((u)* p2));
		position = ((1 - u) * p1) + ((u)* p2);
	}
	else if (u <= 2) {
		R = glm::mat4_cast(glm::normalize(((2 - u) * q2) + ((-1 + u)* q3)));
		MV->translate(((2 - u) * p2) + ((-1 + u)* p3));
		position = (((2 - u) * p2) + ((-1 + u)* p3));
	}
	else if (u <= 3) {
		R = glm::mat4_cast(glm::normalize(((3 - u) * q3) + ((-2 + u)* q4)));
		MV->translate(((3 - u) * p3) + ((-2 + u)* p4));
		position = (((3 - u) * p3) + ((-2 + u)* p4));
	}
	else if (u <= 4) {
		R = glm::mat4_cast(glm::normalize(((4 - u) * q4) + ((-3 + u)* q5)));
		MV->translate(((4 - u) * p4) + ((-3 + u) * p5));
		position = (((4 - u) * p4) + ((-3 + u) * p5));
	}
	else if (u <= 5) {
		R = glm::mat4_cast(glm::normalize(((5 - u) * q5) + ((-4 + u)* q6)));
		MV->translate(((5 - u) * p5) + ((-4 + u) * p6));
		position = (((5 - u) * p5) + ((-4 + u) * p6));
	}
	else if (u <= 6) {
		R = glm::mat4_cast(glm::normalize(((6 - u) * q6) + ((-5 + u)* q7)));
		MV->translate(((6 - u) * p6) + ((-5 + u) * p7));
		position = (((6 - u) * p6) + ((-5 + u) * p7));
	}
	MV->multMatrix(R);
	heli_matrix = catmull->get_mainmatrix(cps, qpos, u);
	glUniformMatrix4fv(progNormal->getUniform("MV"), 1, GL_FALSE, glm::value_ptr(MV->topMatrix()));
	MV->popMatrix();
	
	//Apply Corresponding Color Scheme to helicopter body
	glUniform4f(progNormal->getUniform("lightPos"), -1.0f, 0.0f, 1.0f, 1.0f);
	glUniform3f(progNormal->getUniform("ka"), hbody1color.get_ka().x, hbody1color.get_ka().y, hbody1color.get_ka().z);
	glUniform3f(progNormal->getUniform("kd"), hbody1color.get_kd().x, hbody1color.get_kd().y, hbody1color.get_kd().z);
	glUniform3f(progNormal->getUniform("ks"), hbody1color.get_ks().x, hbody1color.get_ks().y, hbody1color.get_ks().z);
	glUniform1f(progNormal->getUniform("s"), hbody1color.get_s());
	hbody1->draw(progNormal);
	glUniform4f(progNormal->getUniform("lightPos"), -1.0f, 1.0f, 1.0f, 1.0f);
	glUniform3f(progNormal->getUniform("ka"), hbody2color.get_ka().x, hbody2color.get_ka().y, hbody2color.get_ka().z);
	glUniform3f(progNormal->getUniform("kd"), hbody2color.get_kd().x, hbody2color.get_kd().y, hbody2color.get_kd().z);
	glUniform3f(progNormal->getUniform("ks"), hbody2color.get_ks().x, hbody2color.get_ks().y, hbody2color.get_ks().z);
	glUniform1f(progNormal->getUniform("s"), hbody2color.get_s());
	hbody2->draw(progNormal);

	

	glUniform4f(progNormal->getUniform("lightPos"), -1.0f, 1.0f, 1.0f, 1.0f);
	glUniform3f(progNormal->getUniform("ka"), hpropcolor.get_ka().x, hpropcolor.get_ka().y, hpropcolor.get_ka().z);
	glUniform3f(progNormal->getUniform("kd"), hpropcolor.get_kd().x, hpropcolor.get_kd().y, hpropcolor.get_kd().z);
	glUniform3f(progNormal->getUniform("ks"), hpropcolor.get_ks().x, hpropcolor.get_ks().y, hpropcolor.get_ks().z);
	glUniform1f(progNormal->getUniform("s"), hpropcolor.get_s());

	//Push Matrix for Interpolated Top Blade
	MV->pushMatrix();
	if (u <= 1) {
		R = glm::mat4_cast(glm::normalize((1.0f - u) * q1 + u * q2));
		MV->translate(((1 - u) * p1) + ((u)* p2));
	}
	else if (u <= 2) {
		R = glm::mat4_cast(glm::normalize(((2 - u) * q2) + ((-1 + u)* q3)));
		MV->translate(((2 - u) * p2) + ((-1 + u)* p3));
	}
	else if (u <= 3) {
		R = glm::mat4_cast(glm::normalize(((3 - u) * q3) + ((-2 + u)* q4)));
		MV->translate(((3 - u) * p3) + ((-2 + u)* p4));
	}
	else if (u <= 4) {
		R = glm::mat4_cast(glm::normalize(((4 - u) * q4) + ((-3 + u)* q5)));
		MV->translate(((4 - u) * p4) + ((-3 + u) * p5));
	}
	else if (u <= 5) {
		R = glm::mat4_cast(glm::normalize(((5 - u) * q5) + ((-4 + u)* q6)));
		MV->translate(((5 - u) * p5) + ((-4 + u) * p6));
	}
	else if (u <= 6) {
		R = glm::mat4_cast(glm::normalize(((6 - u) * q6) + ((-5 + u)* q7)));
		MV->translate(((6 - u) * p6) + ((-5 + u) * p7));
	}
	MV->multMatrix(R);

	//Translate to Center
	MV->translate(prop1center);
	MV->rotate((float)t, 0.0f, 1.0f, 0.0f);
	MV->translate(-prop1center);
	glUniformMatrix4fv(progNormal->getUniform("MV"), 1, GL_FALSE, glm::value_ptr(MV->topMatrix()));
	
	glUniform4f(progNormal->getUniform("lightPos"), -1.0f, 1.0f, 1.0f, 1.0f);
	glUniform3f(progNormal->getUniform("ka"), hpropcolor.get_ka().x, hpropcolor.get_ka().y, hpropcolor.get_ka().z);
	glUniform3f(progNormal->getUniform("kd"), hpropcolor.get_kd().x, hpropcolor.get_kd().y, hpropcolor.get_kd().z);
	glUniform3f(progNormal->getUniform("ks"), hpropcolor.get_ks().x, hpropcolor.get_ks().y, hpropcolor.get_ks().z);
	glUniform1f(progNormal->getUniform("s"), hpropcolor.get_s());
	hprop1->draw(progNormal);
	MV->popMatrix();
	//Push Matrix for Interpolated Back Blade
	MV->pushMatrix();
	if (u <= 1) {
		R = glm::mat4_cast(glm::normalize((1.0f - u) * q1 + u * q2));
		MV->translate(((1 - u) * p1) + ((u)* p2));
	}
	else if (u <= 2) {
		R = glm::mat4_cast(glm::normalize(((2 - u) * q2) + ((-1 + u)* q3)));
		MV->translate(((2 - u) * p2) + ((-1 + u)* p3));
	}
	else if (u <= 3) {
		R = glm::mat4_cast(glm::normalize(((3 - u) * q3) + ((-2 + u)* q4)));
		MV->translate(((3 - u) * p3) + ((-2 + u)* p4));
	}
	else if (u <= 4) {
		R = glm::mat4_cast(glm::normalize(((4 - u) * q4) + ((-3 + u)* q5)));
		MV->translate(((4 - u) * p4) + ((-3 + u) * p5));
	}
	else if (u <= 5) {
		R = glm::mat4_cast(glm::normalize(((5 - u) * q5) + ((-4 + u)* q6)));
		MV->translate(((5 - u) * p5) + ((-4 + u) * p6));
	}
	else if (u <= 6) {
		R = glm::mat4_cast(glm::normalize(((6 - u) * q6) + ((-5 + u)* q7)));
		MV->translate(((6 - u) * p6) + ((-5 + u) * p7));
	}
	MV->multMatrix(R);
	MV->translate(prop2center);
	MV->rotate((float)t, 0.0f, 0.0f, 1.0f);
	MV->translate(-prop2center);
	glUniformMatrix4fv(progNormal->getUniform("MV"), 1, GL_FALSE, glm::value_ptr(MV->topMatrix()));
	MV->popMatrix();
	
	hprop2->draw(progNormal);
	progNormal->unbind();

	//Bind simple program to draw background grid and centerpiece
	progSimple->bind();

	glUniformMatrix4fv(progSimple->getUniform("P"), 1, GL_FALSE, glm::value_ptr(P->topMatrix()));
	glUniformMatrix4fv(progSimple->getUniform("MV"), 1, GL_FALSE, glm::value_ptr(MV->topMatrix()));
	glLineWidth(2);
	glBegin(GL_LINES);
	glColor3f(1, 0, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(1, 0, 0);
	glColor3f(0, 1, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 1, 0);
	glColor3f(0, 0, 1);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 1);
	glEnd();
	glLineWidth(1);
	//Use for loop to create Background Grid
	glColor3f(1.0f, 0.5f, 0.5f);
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < 100; ++i) {
		for (int j = 0; j < 100; ++j) {
			glVertex3f((float)i - 50, -0.6f, (float)j - 50);
		}
	}
	glEnd();
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < 100; ++i) {
		for (int j = 0; j < 100; ++j) {
			glVertex3f((float)j - 50, -0.6f, (float)i - 50);
		}
	}
	glEnd();


	progSimple->unbind();
	progNormal->bind();
	// Pop stacks

	//Set Correspoinding quat to matrix for rotating model view matrix.
	R = glm::mat4_cast(q0);

	
	//For all individual keyframes below, refer to the interpolated helicopter comments
	//for in depth description about each individual transformation.

	//Activate keyframes if K is pressed
	if (!keyToggles[(unsigned)GLFW_KEY_SPACE]) {
		if (keyToggles[(unsigned)'k']) {
			MV->pushMatrix();
			MV->translate(p0);
			MV->multMatrix(R);
			glUniformMatrix4fv(progNormal->getUniform("MV"), 1, GL_FALSE, glm::value_ptr(MV->topMatrix()));
			MV->popMatrix();
			glUniform4f(progNormal->getUniform("lightPos"), -1.0f, 1.0f, 1.0f, 1.0f);
			glUniform3f(progNormal->getUniform("ka"), hbody1color.get_ka().x, hbody1color.get_ka().y, hbody1color.get_ka().z);
			glUniform3f(progNormal->getUniform("kd"), hbody1color.get_kd().x, hbody1color.get_kd().y, hbody1color.get_kd().z);
			glUniform3f(progNormal->getUniform("ks"), hbody1color.get_ks().x, hbody1color.get_ks().y, hbody1color.get_ks().z);
			glUniform1f(progNormal->getUniform("s"), hbody1color.get_s());
			hbody1->draw(progNormal);
			glUniform4f(progNormal->getUniform("lightPos"), -1.0f, 1.0f, 1.0f, 1.0f);
			glUniform3f(progNormal->getUniform("ka"), hbody2color.get_ka().x, hbody2color.get_ka().y, hbody2color.get_ka().z);
			glUniform3f(progNormal->getUniform("kd"), hbody2color.get_kd().x, hbody2color.get_kd().y, hbody2color.get_kd().z);
			glUniform3f(progNormal->getUniform("ks"), hbody2color.get_ks().x, hbody2color.get_ks().y, hbody2color.get_ks().z);
			glUniform1f(progNormal->getUniform("s"), hbody2color.get_s());
			hbody2->draw(progNormal);
			MV->pushMatrix();
			MV->translate(p0);
			MV->multMatrix(R);
			MV->translate(prop1center);
			MV->rotate((float)t, 0.0f, 1.0f, 0.0f);
			MV->translate(-prop1center);
			glUniformMatrix4fv(progNormal->getUniform("MV"), 1, GL_FALSE, glm::value_ptr(MV->topMatrix()));
			MV->popMatrix();
			glUniform4f(progNormal->getUniform("lightPos"), -1.0f, 1.0f, 1.0f, 1.0f);
			glUniform3f(progNormal->getUniform("ka"), hpropcolor.get_ka().x, hpropcolor.get_ka().y, hpropcolor.get_ka().z);
			glUniform3f(progNormal->getUniform("kd"), hpropcolor.get_kd().x, hpropcolor.get_kd().y, hpropcolor.get_kd().z);
			glUniform3f(progNormal->getUniform("ks"), hpropcolor.get_ks().x, hpropcolor.get_ks().y, hpropcolor.get_ks().z);
			glUniform1f(progNormal->getUniform("s"), hpropcolor.get_s());
			hprop1->draw(progNormal);
			MV->pushMatrix();
			MV->translate(p0);
			MV->multMatrix(R);
			MV->translate(prop2center);
			MV->rotate((float)t, 0.0f, 0.0f, 1.0f);
			MV->translate(-prop2center);
			glUniformMatrix4fv(progNormal->getUniform("MV"), 1, GL_FALSE, glm::value_ptr(MV->topMatrix()));
			MV->popMatrix();
			hprop2->draw(progNormal);

			R = glm::mat4_cast(q1);

			MV->pushMatrix();
			MV->translate(p1);
			MV->multMatrix(R);
			glUniformMatrix4fv(progNormal->getUniform("MV"), 1, GL_FALSE, glm::value_ptr(MV->topMatrix()));
			MV->popMatrix();
			glUniform4f(progNormal->getUniform("lightPos"), -1.0f, 1.0f, 1.0f, 1.0f);
			glUniform3f(progNormal->getUniform("ka"), hbody1color.get_ka().x, hbody1color.get_ka().y, hbody1color.get_ka().z);
			glUniform3f(progNormal->getUniform("kd"), hbody1color.get_kd().x, hbody1color.get_kd().y, hbody1color.get_kd().z);
			glUniform3f(progNormal->getUniform("ks"), hbody1color.get_ks().x, hbody1color.get_ks().y, hbody1color.get_ks().z);
			glUniform1f(progNormal->getUniform("s"), hbody1color.get_s());
			hbody1->draw(progNormal);
			glUniform4f(progNormal->getUniform("lightPos"), -1.0f, 1.0f, 1.0f, 1.0f);
			glUniform3f(progNormal->getUniform("ka"), hbody2color.get_ka().x, hbody2color.get_ka().y, hbody2color.get_ka().z);
			glUniform3f(progNormal->getUniform("kd"), hbody2color.get_kd().x, hbody2color.get_kd().y, hbody2color.get_kd().z);
			glUniform3f(progNormal->getUniform("ks"), hbody2color.get_ks().x, hbody2color.get_ks().y, hbody2color.get_ks().z);
			glUniform1f(progNormal->getUniform("s"), hbody2color.get_s());
			hbody2->draw(progNormal);
			glUniform4f(progNormal->getUniform("lightPos"), -1.0f, 1.0f, 1.0f, 1.0f);
			glUniform3f(progNormal->getUniform("ka"), hpropcolor.get_ka().x, hpropcolor.get_ka().y, hpropcolor.get_ka().z);
			glUniform3f(progNormal->getUniform("kd"), hpropcolor.get_kd().x, hpropcolor.get_kd().y, hpropcolor.get_kd().z);
			glUniform3f(progNormal->getUniform("ks"), hpropcolor.get_ks().x, hpropcolor.get_ks().y, hpropcolor.get_ks().z);
			glUniform1f(progNormal->getUniform("s"), hpropcolor.get_s());
			MV->pushMatrix();
			MV->translate(p1);
			MV->multMatrix(R);
			MV->translate(prop1center);
			MV->rotate((float)t, 0.0f, 1.0f, 0.0f);
			MV->translate(-prop1center);
			glUniformMatrix4fv(progNormal->getUniform("MV"), 1, GL_FALSE, glm::value_ptr(MV->topMatrix()));
			MV->popMatrix();
			glUniform4f(progNormal->getUniform("lightPos"), -1.0f, 1.0f, 1.0f, 1.0f);
			glUniform3f(progNormal->getUniform("ka"), hpropcolor.get_ka().x, hpropcolor.get_ka().y, hpropcolor.get_ka().z);
			glUniform3f(progNormal->getUniform("kd"), hpropcolor.get_kd().x, hpropcolor.get_kd().y, hpropcolor.get_kd().z);
			glUniform3f(progNormal->getUniform("ks"), hpropcolor.get_ks().x, hpropcolor.get_ks().y, hpropcolor.get_ks().z);
			glUniform1f(progNormal->getUniform("s"), hpropcolor.get_s());
			hprop1->draw(progNormal);
			MV->pushMatrix();
			MV->translate(p1);
			MV->multMatrix(R);
			MV->translate(prop2center);
			MV->rotate((float)t, 0.0f, 0.0f, 1.0f);
			MV->translate(-prop2center);
			glUniformMatrix4fv(progNormal->getUniform("MV"), 1, GL_FALSE, glm::value_ptr(MV->topMatrix()));
			MV->popMatrix();
			hprop2->draw(progNormal);

			R = glm::mat4_cast(q2);

			MV->pushMatrix();
			MV->translate(p2);
			MV->multMatrix(R);
			glUniformMatrix4fv(progNormal->getUniform("MV"), 1, GL_FALSE, glm::value_ptr(MV->topMatrix()));
			MV->popMatrix();
			glUniform4f(progNormal->getUniform("lightPos"), -1.0f, 1.0f, 1.0f, 1.0f);
			glUniform3f(progNormal->getUniform("ka"), hbody1color.get_ka().x, hbody1color.get_ka().y, hbody1color.get_ka().z);
			glUniform3f(progNormal->getUniform("kd"), hbody1color.get_kd().x, hbody1color.get_kd().y, hbody1color.get_kd().z);
			glUniform3f(progNormal->getUniform("ks"), hbody1color.get_ks().x, hbody1color.get_ks().y, hbody1color.get_ks().z);
			glUniform1f(progNormal->getUniform("s"), hbody1color.get_s());
			hbody1->draw(progNormal);
			glUniform4f(progNormal->getUniform("lightPos"), -1.0f, 1.0f, 1.0f, 1.0f);
			glUniform3f(progNormal->getUniform("ka"), hbody2color.get_ka().x, hbody2color.get_ka().y, hbody2color.get_ka().z);
			glUniform3f(progNormal->getUniform("kd"), hbody2color.get_kd().x, hbody2color.get_kd().y, hbody2color.get_kd().z);
			glUniform3f(progNormal->getUniform("ks"), hbody2color.get_ks().x, hbody2color.get_ks().y, hbody2color.get_ks().z);
			glUniform1f(progNormal->getUniform("s"), hbody2color.get_s());
			hbody2->draw(progNormal);
			glUniform4f(progNormal->getUniform("lightPos"), -1.0f, 1.0f, 1.0f, 1.0f);
			glUniform3f(progNormal->getUniform("ka"), hpropcolor.get_ka().x, hpropcolor.get_ka().y, hpropcolor.get_ka().z);
			glUniform3f(progNormal->getUniform("kd"), hpropcolor.get_kd().x, hpropcolor.get_kd().y, hpropcolor.get_kd().z);
			glUniform3f(progNormal->getUniform("ks"), hpropcolor.get_ks().x, hpropcolor.get_ks().y, hpropcolor.get_ks().z);
			glUniform1f(progNormal->getUniform("s"), hpropcolor.get_s());
			MV->pushMatrix();
			MV->translate(p2);
			MV->multMatrix(R);
			MV->translate(prop1center);
			MV->rotate((float)t, 0.0f, 1.0f, 0.0f);
			MV->translate(-prop1center);
			glUniformMatrix4fv(progNormal->getUniform("MV"), 1, GL_FALSE, glm::value_ptr(MV->topMatrix()));
			MV->popMatrix();
			glUniform4f(progNormal->getUniform("lightPos"), -1.0f, 1.0f, 1.0f, 1.0f);
			glUniform3f(progNormal->getUniform("ka"), hpropcolor.get_ka().x, hpropcolor.get_ka().y, hpropcolor.get_ka().z);
			glUniform3f(progNormal->getUniform("kd"), hpropcolor.get_kd().x, hpropcolor.get_kd().y, hpropcolor.get_kd().z);
			glUniform3f(progNormal->getUniform("ks"), hpropcolor.get_ks().x, hpropcolor.get_ks().y, hpropcolor.get_ks().z);
			glUniform1f(progNormal->getUniform("s"), hpropcolor.get_s());
			hprop1->draw(progNormal);
			MV->pushMatrix();
			MV->translate(p2);
			MV->multMatrix(R);
			MV->translate(prop2center);
			MV->rotate((float)t, 0.0f, 0.0f, 1.0f);
			MV->translate(-prop2center);
			glUniformMatrix4fv(progNormal->getUniform("MV"), 1, GL_FALSE, glm::value_ptr(MV->topMatrix()));
			MV->popMatrix();
			hprop2->draw(progNormal);

			R = glm::mat4_cast(q3);


			MV->pushMatrix();
			MV->translate(p3);
			MV->multMatrix(R);
			glUniformMatrix4fv(progNormal->getUniform("MV"), 1, GL_FALSE, glm::value_ptr(MV->topMatrix()));
			MV->popMatrix();
			glUniform4f(progNormal->getUniform("lightPos"), -1.0f, 1.0f, 1.0f, 1.0f);
			glUniform3f(progNormal->getUniform("ka"), hbody1color.get_ka().x, hbody1color.get_ka().y, hbody1color.get_ka().z);
			glUniform3f(progNormal->getUniform("kd"), hbody1color.get_kd().x, hbody1color.get_kd().y, hbody1color.get_kd().z);
			glUniform3f(progNormal->getUniform("ks"), hbody1color.get_ks().x, hbody1color.get_ks().y, hbody1color.get_ks().z);
			glUniform1f(progNormal->getUniform("s"), hbody1color.get_s());
			hbody1->draw(progNormal);
			glUniform4f(progNormal->getUniform("lightPos"), -1.0f, 1.0f, 1.0f, 1.0f);
			glUniform3f(progNormal->getUniform("ka"), hbody2color.get_ka().x, hbody2color.get_ka().y, hbody2color.get_ka().z);
			glUniform3f(progNormal->getUniform("kd"), hbody2color.get_kd().x, hbody2color.get_kd().y, hbody2color.get_kd().z);
			glUniform3f(progNormal->getUniform("ks"), hbody2color.get_ks().x, hbody2color.get_ks().y, hbody2color.get_ks().z);
			glUniform1f(progNormal->getUniform("s"), hbody2color.get_s());
			hbody2->draw(progNormal);
			glUniform4f(progNormal->getUniform("lightPos"), -1.0f, 1.0f, 1.0f, 1.0f);
			glUniform3f(progNormal->getUniform("ka"), hpropcolor.get_ka().x, hpropcolor.get_ka().y, hpropcolor.get_ka().z);
			glUniform3f(progNormal->getUniform("kd"), hpropcolor.get_kd().x, hpropcolor.get_kd().y, hpropcolor.get_kd().z);
			glUniform3f(progNormal->getUniform("ks"), hpropcolor.get_ks().x, hpropcolor.get_ks().y, hpropcolor.get_ks().z);
			glUniform1f(progNormal->getUniform("s"), hpropcolor.get_s());
			MV->pushMatrix();
			MV->translate(p3);
			MV->multMatrix(R);
			MV->translate(prop1center);
			MV->rotate((float)t, 0.0f, 1.0f, 0.0f);
			MV->translate(-prop1center);
			glUniformMatrix4fv(progNormal->getUniform("MV"), 1, GL_FALSE, glm::value_ptr(MV->topMatrix()));
			MV->popMatrix();
			glUniform4f(progNormal->getUniform("lightPos"), -1.0f, 1.0f, 1.0f, 1.0f);
			glUniform3f(progNormal->getUniform("ka"), hpropcolor.get_ka().x, hpropcolor.get_ka().y, hpropcolor.get_ka().z);
			glUniform3f(progNormal->getUniform("kd"), hpropcolor.get_kd().x, hpropcolor.get_kd().y, hpropcolor.get_kd().z);
			glUniform3f(progNormal->getUniform("ks"), hpropcolor.get_ks().x, hpropcolor.get_ks().y, hpropcolor.get_ks().z);
			glUniform1f(progNormal->getUniform("s"), hpropcolor.get_s());
			hprop1->draw(progNormal);
			MV->pushMatrix();
			MV->translate(p3);
			MV->multMatrix(R);
			MV->translate(prop2center);
			MV->rotate((float)t, 0.0f, 0.0f, 1.0f);
			MV->translate(-prop2center);
			glUniformMatrix4fv(progNormal->getUniform("MV"), 1, GL_FALSE, glm::value_ptr(MV->topMatrix()));
			MV->popMatrix();
			hprop2->draw(progNormal);

			R = glm::mat4_cast(q4);


			MV->pushMatrix();
			MV->translate(p4);
			MV->multMatrix(R);
			glUniformMatrix4fv(progNormal->getUniform("MV"), 1, GL_FALSE, glm::value_ptr(MV->topMatrix()));
			MV->popMatrix();
			glUniform4f(progNormal->getUniform("lightPos"), -1.0f, 1.0f, 1.0f, 1.0f);
			glUniform3f(progNormal->getUniform("ka"), hbody1color.get_ka().x, hbody1color.get_ka().y, hbody1color.get_ka().z);
			glUniform3f(progNormal->getUniform("kd"), hbody1color.get_kd().x, hbody1color.get_kd().y, hbody1color.get_kd().z);
			glUniform3f(progNormal->getUniform("ks"), hbody1color.get_ks().x, hbody1color.get_ks().y, hbody1color.get_ks().z);
			glUniform1f(progNormal->getUniform("s"), hbody1color.get_s());
			hbody1->draw(progNormal);
			glUniform4f(progNormal->getUniform("lightPos"), -1.0f, 1.0f, 1.0f, 1.0f);
			glUniform3f(progNormal->getUniform("ka"), hbody2color.get_ka().x, hbody2color.get_ka().y, hbody2color.get_ka().z);
			glUniform3f(progNormal->getUniform("kd"), hbody2color.get_kd().x, hbody2color.get_kd().y, hbody2color.get_kd().z);
			glUniform3f(progNormal->getUniform("ks"), hbody2color.get_ks().x, hbody2color.get_ks().y, hbody2color.get_ks().z);
			glUniform1f(progNormal->getUniform("s"), hbody2color.get_s());
			hbody2->draw(progNormal);
			glUniform4f(progNormal->getUniform("lightPos"), -1.0f, 1.0f, 1.0f, 1.0f);
			glUniform3f(progNormal->getUniform("ka"), hpropcolor.get_ka().x, hpropcolor.get_ka().y, hpropcolor.get_ka().z);
			glUniform3f(progNormal->getUniform("kd"), hpropcolor.get_kd().x, hpropcolor.get_kd().y, hpropcolor.get_kd().z);
			glUniform3f(progNormal->getUniform("ks"), hpropcolor.get_ks().x, hpropcolor.get_ks().y, hpropcolor.get_ks().z);
			glUniform1f(progNormal->getUniform("s"), hpropcolor.get_s());
			MV->pushMatrix();
			MV->translate(p4);
			MV->multMatrix(R);
			MV->translate(prop1center);
			MV->rotate((float)t, 0.0f, 1.0f, 0.0f);
			MV->translate(-prop1center);
			glUniformMatrix4fv(progNormal->getUniform("MV"), 1, GL_FALSE, glm::value_ptr(MV->topMatrix()));
			MV->popMatrix();
			glUniform4f(progNormal->getUniform("lightPos"), -1.0f, 1.0f, 1.0f, 1.0f);
			glUniform3f(progNormal->getUniform("ka"), hpropcolor.get_ka().x, hpropcolor.get_ka().y, hpropcolor.get_ka().z);
			glUniform3f(progNormal->getUniform("kd"), hpropcolor.get_kd().x, hpropcolor.get_kd().y, hpropcolor.get_kd().z);
			glUniform3f(progNormal->getUniform("ks"), hpropcolor.get_ks().x, hpropcolor.get_ks().y, hpropcolor.get_ks().z);
			glUniform1f(progNormal->getUniform("s"), hpropcolor.get_s());
			hprop1->draw(progNormal);
			MV->pushMatrix();
			MV->translate(p4);
			MV->multMatrix(R);
			MV->translate(prop2center);
			MV->rotate((float)t, 0.0f, 0.0f, 1.0f);
			MV->translate(-prop2center);
			glUniformMatrix4fv(progNormal->getUniform("MV"), 1, GL_FALSE, glm::value_ptr(MV->topMatrix()));
			MV->popMatrix();
			hprop2->draw(progNormal);

			R = glm::mat4_cast(q5);


			MV->pushMatrix();
			MV->translate(p5);
			MV->multMatrix(R);
			glUniformMatrix4fv(progNormal->getUniform("MV"), 1, GL_FALSE, glm::value_ptr(MV->topMatrix()));
			MV->popMatrix();
			glUniform4f(progNormal->getUniform("lightPos"), -1.0f, 1.0f, 1.0f, 1.0f);
			glUniform3f(progNormal->getUniform("ka"), hbody1color.get_ka().x, hbody1color.get_ka().y, hbody1color.get_ka().z);
			glUniform3f(progNormal->getUniform("kd"), hbody1color.get_kd().x, hbody1color.get_kd().y, hbody1color.get_kd().z);
			glUniform3f(progNormal->getUniform("ks"), hbody1color.get_ks().x, hbody1color.get_ks().y, hbody1color.get_ks().z);
			glUniform1f(progNormal->getUniform("s"), hbody1color.get_s());
			hbody1->draw(progNormal);
			glUniform4f(progNormal->getUniform("lightPos"), -1.0f, 1.0f, 1.0f, 1.0f);
			glUniform3f(progNormal->getUniform("ka"), hbody2color.get_ka().x, hbody2color.get_ka().y, hbody2color.get_ka().z);
			glUniform3f(progNormal->getUniform("kd"), hbody2color.get_kd().x, hbody2color.get_kd().y, hbody2color.get_kd().z);
			glUniform3f(progNormal->getUniform("ks"), hbody2color.get_ks().x, hbody2color.get_ks().y, hbody2color.get_ks().z);
			glUniform1f(progNormal->getUniform("s"), hbody2color.get_s());
			hbody2->draw(progNormal);
			glUniform4f(progNormal->getUniform("lightPos"), -1.0f, 1.0f, 1.0f, 1.0f);
			glUniform3f(progNormal->getUniform("ka"), hpropcolor.get_ka().x, hpropcolor.get_ka().y, hpropcolor.get_ka().z);
			glUniform3f(progNormal->getUniform("kd"), hpropcolor.get_kd().x, hpropcolor.get_kd().y, hpropcolor.get_kd().z);
			glUniform3f(progNormal->getUniform("ks"), hpropcolor.get_ks().x, hpropcolor.get_ks().y, hpropcolor.get_ks().z);
			glUniform1f(progNormal->getUniform("s"), hpropcolor.get_s());
			MV->pushMatrix();
			MV->translate(p5);
			MV->multMatrix(R);
			MV->translate(prop1center);
			MV->rotate((float)t, 0.0f, 1.0f, 0.0f);
			MV->translate(-prop1center);
			glUniformMatrix4fv(progNormal->getUniform("MV"), 1, GL_FALSE, glm::value_ptr(MV->topMatrix()));
			MV->popMatrix();
			glUniform4f(progNormal->getUniform("lightPos"), -1.0f, 1.0f, 1.0f, 1.0f);
			glUniform3f(progNormal->getUniform("ka"), hpropcolor.get_ka().x, hpropcolor.get_ka().y, hpropcolor.get_ka().z);
			glUniform3f(progNormal->getUniform("kd"), hpropcolor.get_kd().x, hpropcolor.get_kd().y, hpropcolor.get_kd().z);
			glUniform3f(progNormal->getUniform("ks"), hpropcolor.get_ks().x, hpropcolor.get_ks().y, hpropcolor.get_ks().z);
			glUniform1f(progNormal->getUniform("s"), hpropcolor.get_s());
			hprop1->draw(progNormal);
			MV->pushMatrix();
			MV->translate(p5);
			MV->multMatrix(R);
			MV->translate(prop2center);
			MV->rotate((float)t, 0.0f, 0.0f, 1.0f);
			MV->translate(-prop2center);
			glUniformMatrix4fv(progNormal->getUniform("MV"), 1, GL_FALSE, glm::value_ptr(MV->topMatrix()));
			MV->popMatrix();
			hprop2->draw(progNormal);

			R = glm::mat4_cast(q6);

			MV->pushMatrix();
			MV->translate(p6);
			MV->multMatrix(R);
			glUniformMatrix4fv(progNormal->getUniform("MV"), 1, GL_FALSE, glm::value_ptr(MV->topMatrix()));
			MV->popMatrix();
			glUniform4f(progNormal->getUniform("lightPos"), -1.0f, 1.0f, 1.0f, 1.0f);
			glUniform3f(progNormal->getUniform("ka"), hbody1color.get_ka().x, hbody1color.get_ka().y, hbody1color.get_ka().z);
			glUniform3f(progNormal->getUniform("kd"), hbody1color.get_kd().x, hbody1color.get_kd().y, hbody1color.get_kd().z);
			glUniform3f(progNormal->getUniform("ks"), hbody1color.get_ks().x, hbody1color.get_ks().y, hbody1color.get_ks().z);
			glUniform1f(progNormal->getUniform("s"), hbody1color.get_s());
			hbody1->draw(progNormal);
			glUniform4f(progNormal->getUniform("lightPos"), -1.0f, 1.0f, 1.0f, 1.0f);
			glUniform3f(progNormal->getUniform("ka"), hbody2color.get_ka().x, hbody2color.get_ka().y, hbody2color.get_ka().z);
			glUniform3f(progNormal->getUniform("kd"), hbody2color.get_kd().x, hbody2color.get_kd().y, hbody2color.get_kd().z);
			glUniform3f(progNormal->getUniform("ks"), hbody2color.get_ks().x, hbody2color.get_ks().y, hbody2color.get_ks().z);
			glUniform1f(progNormal->getUniform("s"), hbody2color.get_s());
			hbody2->draw(progNormal);
			glUniform4f(progNormal->getUniform("lightPos"), -1.0f, 1.0f, 1.0f, 1.0f);
			glUniform3f(progNormal->getUniform("ka"), hpropcolor.get_ka().x, hpropcolor.get_ka().y, hpropcolor.get_ka().z);
			glUniform3f(progNormal->getUniform("kd"), hpropcolor.get_kd().x, hpropcolor.get_kd().y, hpropcolor.get_kd().z);
			glUniform3f(progNormal->getUniform("ks"), hpropcolor.get_ks().x, hpropcolor.get_ks().y, hpropcolor.get_ks().z);
			glUniform1f(progNormal->getUniform("s"), hpropcolor.get_s());
			MV->pushMatrix();
			MV->translate(p6);
			MV->multMatrix(R);
			MV->translate(prop1center);
			MV->rotate((float)t, 0.0f, 1.0f, 0.0f);
			MV->translate(-prop1center);
			glUniformMatrix4fv(progNormal->getUniform("MV"), 1, GL_FALSE, glm::value_ptr(MV->topMatrix()));
			MV->popMatrix();
			glUniform4f(progNormal->getUniform("lightPos"), -1.0f, 1.0f, 1.0f, 1.0f);
			glUniform3f(progNormal->getUniform("ka"), hpropcolor.get_ka().x, hpropcolor.get_ka().y, hpropcolor.get_ka().z);
			glUniform3f(progNormal->getUniform("kd"), hpropcolor.get_kd().x, hpropcolor.get_kd().y, hpropcolor.get_kd().z);
			glUniform3f(progNormal->getUniform("ks"), hpropcolor.get_ks().x, hpropcolor.get_ks().y, hpropcolor.get_ks().z);
			glUniform1f(progNormal->getUniform("s"), hpropcolor.get_s());
			hprop1->draw(progNormal);
			MV->pushMatrix();
			MV->translate(p6);
			MV->multMatrix(R);
			MV->translate(prop2center);
			MV->rotate((float)t, 0.0f, 0.0f, 1.0f);
			MV->translate(-prop2center);
			glUniformMatrix4fv(progNormal->getUniform("MV"), 1, GL_FALSE, glm::value_ptr(MV->topMatrix()));
			MV->popMatrix();
			hprop2->draw(progNormal);
		}
	}
	
	


	
	
	progNormal->unbind();

	

	progSimple->bind();

	catmull->build_curve(cps);
	catmull->buildTable(cps, qpos);
	s_temp = catmull->t2s((float)t);
	u = catmull->s2u(s_temp);
	u_marker = catmull->get_umarker() / 6.0f;
	progSimple->unbind();

	

	MV->popMatrix();
	P->popMatrix();
	
	GLSL::checkError(GET_FILE_LINE);
}

int main(int argc, char **argv)
{
	if(argc < 2) {
		cout << "Please specify the resource directory." << endl;
		return 0;
	}
	RESOURCE_DIR = argv[1] + string("/");
	
	// Set error callback.
	glfwSetErrorCallback(error_callback);
	// Initialize the library.
	if(!glfwInit()) {
		return -1;
	}
	// Create a windowed mode window and its OpenGL context.
	window = glfwCreateWindow(640, 480, "YOUR NAME", NULL, NULL);
	if(!window) {
		glfwTerminate();
		return -1;
	}
	// Make the window's context current.
	glfwMakeContextCurrent(window);
	// Initialize GLEW.
	glewExperimental = true;
	if(glewInit() != GLEW_OK) {
		cerr << "Failed to initialize GLEW" << endl;
		return -1;
	}
	glGetError(); // A bug in glewInit() causes an error that we can safely ignore.
	cout << "OpenGL version: " << glGetString(GL_VERSION) << endl;
	cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
	// Set vsync.
	glfwSwapInterval(1);
	// Set keyboard callback.
	glfwSetKeyCallback(window, key_callback);
	// Set char callback.
	glfwSetCharCallback(window, char_callback);
	// Set cursor position callback.
	glfwSetCursorPosCallback(window, cursor_position_callback);
	// Set mouse button callback.
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	// Initialize scene.
	init();
	// Loop until the user closes the window.
	while(!glfwWindowShouldClose(window)) {
		// Render scene.
		render();
		// Swap front and back buffers.
		glfwSwapBuffers(window);
		// Poll for and process events.
		glfwPollEvents();
	}
	// Quit program.
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
