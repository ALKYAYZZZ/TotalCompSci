#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>

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
#include "Component.h"

using namespace std;

GLFWwindow *window; // Main application window
string RES_DIR = ""; // Where data files live
shared_ptr<Program> prog, prog1;
shared_ptr<Shape> shape;

//Create Components
vector<Component*> comp_list;
Component * torso = new Component(true);
Component * head = new Component(false);
Component * upper_left_arm = new Component(false);
Component * lower_left_arm = new Component(false);
Component * upper_right_arm = new Component(false);
Component * lower_right_arm = new Component(false);
Component * upper_left_leg = new Component(false);
Component * lower_left_leg = new Component(false);
Component * upper_right_leg = new Component(false);
Component * lower_right_leg = new Component(false);

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



static void init()
{
	GLSL::checkVersion();

	// Set background color.
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	// Enable z-buffer test.
	glEnable(GL_DEPTH_TEST);

	// Initialize mesh.
	shape = make_shared<Shape>();
	shape->loadMesh(RES_DIR + "cube.obj");
	shape->init();

	prog = make_shared<Program>();
	prog->setVerbose(true);
	prog->setShaderNames(RES_DIR + "vert.glsl", RES_DIR + "frag.glsl");
	prog->init();
	prog->addUniform("P");
	prog->addUniform("MV");
	prog->addAttribute("aPos");
	prog->addAttribute("aNor");
	prog->addAttribute("aTex");
	

	// Get current frame buffer size.
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	float aspect = width / (float)height;
	glViewport(0, 0, width, height);

	//Set Components
	torso->add_child(head);
	torso->add_child(upper_left_arm);
	torso->add_child(upper_right_arm);
	torso->add_child(upper_left_leg);
	torso->add_child(upper_right_leg);
	upper_left_arm->add_child(lower_left_arm);
	upper_right_arm->add_child(lower_right_arm);
	upper_left_leg->add_child(lower_left_leg);
	upper_right_leg->add_child(lower_right_leg);


	// Clear framebuffer.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Create matrix stacks.
	auto P = make_shared<MatrixStack>();
	auto MV = make_shared<MatrixStack>();

	// Apply projection.
	P->pushMatrix();
	P->multMatrix(glm::perspective((float)(45.0*M_PI / 180.0), aspect, 0.01f, 100.0f));

	//Bind Program and Set Projection Matrix to top of stack
	prog->bind();
	glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, &P->topMatrix()[0][0]);


	// Apply camera transform.
	MV->pushMatrix();
		//Global Transformations
		MV->translate(glm::vec3(0, 0, -8));
		MV->rotate(1, 0, 1, 0);
		glUniformMatrix4fv(prog->getUniform("MV"), 1, GL_FALSE, &MV->topMatrix()[0][0]);
			MV->pushMatrix();
			//Make Torso
			MV->scale(glm::vec3(1, 2, .75));
			torso->set_translation_to_mesh(glm::vec3(0, 0, 0));
			torso->set_scaling_factors(glm::vec3(1, 2, .75));
			glm::vec3 torso_joint = glm::vec3(0, 0, 0);
			torso->set_translation_to_parent(torso_joint, torso_joint);
			torso->select();
			torso->set_joint_angles(glm::vec3(0, 0, 0));
			glUniformMatrix4fv(prog->getUniform("MV"), 1, GL_FALSE, &MV->topMatrix()[0][0]);;
			shape->draw(prog);
		MV->popMatrix();
			MV->pushMatrix();
			//Make Head
			MV->translate(glm::vec3(0, 1.25, 0));
			MV->scale(glm::vec3(.5, .5, .5));
			head->set_scaling_factors(glm::vec3(.5, .5, .5));
			glm::vec3 head_joint = head->calculate_joint_location(glm::vec3(0, 1.25, 0), 1);
			head->set_translation_to_mesh(glm::vec3(0, 1.25, 0), head_joint);
			head->set_translation_to_parent(head_joint, torso_joint);
			head->set_joint_angles(glm::vec3(0, 0, 0));
			glUniformMatrix4fv(prog->getUniform("MV"), 1, GL_FALSE, &MV->topMatrix()[0][0]);
			shape->draw(prog);
		MV->popMatrix();
			MV->pushMatrix();
			//Make Upper Right Arm
			MV->translate(glm::vec3(-1, .75, 0));
			MV->scale(glm::vec3(1, .33, .5));
			upper_right_arm->set_scaling_factors(glm::vec3(1, .33, .5));
			glm::vec3 upper_right_arm_joint = upper_right_arm->calculate_joint_location(glm::vec3(-1, .75, 0), 3);
			upper_right_arm->set_translation_to_mesh(glm::vec3(-1, .75, 0), upper_right_arm_joint);
			upper_right_arm->set_translation_to_parent(upper_right_arm_joint, torso_joint);
			upper_right_arm->set_joint_angles(glm::vec3(0, 0, 0));
			glUniformMatrix4fv(prog->getUniform("MV"), 1, GL_FALSE, &MV->topMatrix()[0][0]);
			shape->draw(prog);
		MV->popMatrix();
			MV->pushMatrix();
			//Make Lower Right Arm
			MV->translate(glm::vec3(-2, .75, 0));
			MV->scale(glm::vec3(1, .25, .33));
			lower_right_arm->set_scaling_factors(glm::vec3(1, .25, .33));
			glm::vec3 lower_right_arm_joint = lower_right_arm->calculate_joint_location(glm::vec3(-2, .75, 0), 3);
			lower_right_arm->set_translation_to_mesh(glm::vec3(-2, .75, 0), lower_right_arm_joint);
			lower_right_arm->set_translation_to_parent(lower_right_arm_joint, upper_right_arm_joint);
			lower_right_arm->set_joint_angles(glm::vec3(0, 0, 0));
			glUniformMatrix4fv(prog->getUniform("MV"), 1, GL_FALSE, &MV->topMatrix()[0][0]);
			shape->draw(prog);
		MV->popMatrix();
			MV->pushMatrix();
			//Make Upper Left Arm
			MV->translate(glm::vec3(1, .75, 0));
			MV->scale(glm::vec3(1, .33, .5));
			upper_left_arm->set_scaling_factors(glm::vec3(1, .33, .5));
			glm::vec3 upper_left_arm_joint = upper_left_arm->calculate_joint_location(glm::vec3(1, .75, 0), 2);
			upper_left_arm->set_translation_to_mesh(glm::vec3(1, .75, 0), upper_left_arm_joint);
			upper_left_arm->set_translation_to_parent(upper_left_arm_joint, torso_joint);
			upper_left_arm->set_joint_angles(glm::vec3(0, 0, 0));
			glUniformMatrix4fv(prog->getUniform("MV"), 1, GL_FALSE, &MV->topMatrix()[0][0]);
			shape->draw(prog);
		MV->popMatrix();
			MV->pushMatrix();
			//Make Lower Left Arm
			MV->translate(glm::vec3(2, .75, 0));
			MV->scale(glm::vec3(1, .25, .33));
			lower_left_arm->set_scaling_factors(glm::vec3(1, .25, .33));
			glm::vec3 lower_left_arm_joint = lower_left_arm->calculate_joint_location(glm::vec3(2, .75, 0), 2);
			lower_left_arm->set_translation_to_mesh(glm::vec3(2, .75, 0), lower_left_arm_joint);
			lower_left_arm->set_translation_to_parent(lower_left_arm_joint, upper_left_arm_joint);
			lower_left_arm->set_joint_angles(glm::vec3(0, 0, 0));
			glUniformMatrix4fv(prog->getUniform("MV"), 1, GL_FALSE, &MV->topMatrix()[0][0]);
			shape->draw(prog);
		MV->popMatrix();
			MV->pushMatrix();
			//Make Upper Right Leg
			MV->translate(glm::vec3(-.25, -1.5, 0));
			MV->scale(glm::vec3(.4, 1, .6));
			upper_right_leg->set_scaling_factors(glm::vec3(.4, 1, .6));
			glm::vec3 upper_right_leg_joint = upper_right_leg->calculate_joint_location(glm::vec3(-.25, -1.5, 0), 5);
			upper_right_leg->set_translation_to_mesh(glm::vec3(-.25, -1.5, 0), upper_right_leg_joint);
			upper_right_leg->set_translation_to_parent(upper_right_leg_joint, torso_joint);
			upper_right_leg->set_joint_angles(glm::vec3(0, 0, 0));
			glUniformMatrix4fv(prog->getUniform("MV"), 1, GL_FALSE, &MV->topMatrix()[0][0]);
			shape->draw(prog);
		MV->popMatrix();
			MV->pushMatrix();
			//Make Lower Right Leg
			MV->translate(glm::vec3(-.25, -2.5, 0));
			MV->scale(glm::vec3(.35, 1, .5));
			lower_right_leg->set_scaling_factors(glm::vec3(.35, 1, .5));
			glm::vec3 lower_right_leg_joint = lower_right_leg->calculate_joint_location(glm::vec3(-.25, -2.5, 0), 5);
			lower_right_leg->set_translation_to_mesh(glm::vec3(-.25, -2.5, 0), lower_right_leg_joint);
			lower_right_leg->set_translation_to_parent(lower_right_leg_joint, upper_right_leg_joint);
			lower_right_leg->set_joint_angles(glm::vec3(0, 0, 0));
			glUniformMatrix4fv(prog->getUniform("MV"), 1, GL_FALSE, &MV->topMatrix()[0][0]);
			shape->draw(prog);
		MV->popMatrix();
			MV->pushMatrix();
			//Make Upper Left Leg
			MV->translate(glm::vec3(.25, -1.5, 0));
			MV->scale(glm::vec3(.4, 1, .6));
			upper_left_leg->set_scaling_factors(glm::vec3(.4, 1, .6));
			glm::vec3 upper_left_leg_joint = upper_left_leg->calculate_joint_location(glm::vec3(.25, -1.5, 0), 4);
			upper_left_leg->set_translation_to_mesh(glm::vec3(.25, -1.5, 0), upper_left_leg_joint);
			upper_left_leg->set_translation_to_parent(upper_left_leg_joint, torso_joint);
			upper_left_leg->set_joint_angles(glm::vec3(0, 0, 0));
			glUniformMatrix4fv(prog->getUniform("MV"), 1, GL_FALSE, &MV->topMatrix()[0][0]);
			shape->draw(prog);
		MV->popMatrix();
			MV->pushMatrix();
			//Make Lower Left Leg
			MV->translate(glm::vec3(.25, -2.5, 0));
			MV->scale(glm::vec3(.35, 1, .5));
			lower_left_leg->set_scaling_factors(glm::vec3(.35, 1, .5));
			glm::vec3 lower_left_leg_joint = lower_left_leg->calculate_joint_location(glm::vec3(.25, -2.5, 0), 4);
			lower_left_leg->set_translation_to_mesh(glm::vec3(.25, -2.5, 0), lower_left_leg_joint);
			lower_left_leg->set_translation_to_parent(lower_left_leg_joint, upper_left_leg_joint);
			lower_left_leg->set_joint_angles(glm::vec3(0, 0, 0));
			glUniformMatrix4fv(prog->getUniform("MV"), 1, GL_FALSE, &MV->topMatrix()[0][0]);
			shape->draw(prog);
		MV->popMatrix();
	
	//Unbind Program
	prog->unbind();

	// Pop matrix stacks.
	MV->popMatrix();
	P->popMatrix();


	prog->setVerbose(false);
	
	// If there were any OpenGL errors, this will print something.
	// You can intersperse this line in your code to find the exact location
	// of your OpenGL error.
	GLSL::checkError(GET_FILE_LINE);
}

static void render()
{
	// Get current frame buffer size.
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	float aspect = width / (float)height;
	glViewport(0, 0, width, height);

	// Clear framebuffer.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Create matrix stacks.
	auto P = make_shared<MatrixStack>();
	auto MV = make_shared<MatrixStack>();

	// Apply projection.
	P->pushMatrix();
	P->multMatrix(glm::perspective((float)(45.0*M_PI / 180.0), aspect, 0.01f, 100.0f));

	//Bind Program and Set Projection Matrix to top of stack
	prog->bind();
	glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, &P->topMatrix()[0][0]);


	MV->pushMatrix();
		//Global Transformations
		MV->translate(glm::vec3(0, 0, -8));
		glUniformMatrix4fv(prog->getUniform("MV"), 1, GL_FALSE, &MV->topMatrix()[0][0]);
			comp_list[0]->draw(MV, prog, shape, RES_DIR);
	//Pop Projection and Camera matricies and unbind program.
	MV->popMatrix();
	P->popMatrix();
	prog->unbind();

	GLSL::checkError(GET_FILE_LINE);
}

static void character_callback(GLFWwindow* window, unsigned int codepoint)
{
	//Recursive function for increasing heirarchy
	if (codepoint == ',') {
		torso->move_selection_down(NULL, false);
	}
	//Recursive function for decreasing heirarchy
	else if (codepoint == '.') {
		torso->move_selection_up(lower_right_leg, false);
	}
	//Decrement x rotation value
	else if (codepoint == 'x') {
		for (size_t i = 0; i < comp_list.size(); i++) {
			if ((comp_list[i])->is_selected() == true) {
				comp_list[i]->decrement_x();
			}
		}
	}
	//Increment x rotation value
	else if (codepoint == 'X') {
		for (size_t i = 0; i < comp_list.size(); i++) {
			if ((comp_list[i])->is_selected() == true) {
				comp_list[i]->increment_x();
			}
		}
	}
	//Decrement y rotation value
	else if (codepoint == 'y') {
		for (size_t i = 0; i < comp_list.size(); i++) {
			if ((comp_list[i])->is_selected() == true) {
				comp_list[i]->decrement_y();
			}
		}
	}
	//Increment y rotation value
	else if (codepoint == 'Y') {
		for (size_t i = 0; i < comp_list.size(); i++) {
			if ((comp_list[i])->is_selected() == true) {
				comp_list[i]->increment_y();
			}
		}
	}
	//Decrememnt z rotation value
	else if (codepoint == 'z') {
		for (size_t i = 0; i < comp_list.size(); i++) {
			if ((comp_list[i])->is_selected() == true) {
				comp_list[i]->decrement_z();
			}
		}
	}
	//Increment Z rotation value
	else if (codepoint == 'Z') {
		for (size_t i = 0; i < comp_list.size(); i++) {
			if ((comp_list[i])->is_selected() == true) {
				comp_list[i]->increment_z();
			}
		}
	}
}

int main(int argc, char **argv)
{
	//Add all components to vector for rotation functions
	comp_list.push_back(torso);
	comp_list.push_back(head);
	comp_list.push_back(upper_left_arm);
	comp_list.push_back(lower_left_arm);
	comp_list.push_back(upper_right_arm);
	comp_list.push_back(lower_right_arm);
	comp_list.push_back(upper_left_leg);
	comp_list.push_back(lower_left_leg);
	comp_list.push_back(upper_right_leg);
	comp_list.push_back(lower_right_leg);
	
	if(argc < 2) {
		cout << "Please specify the resource directory." << endl;
		return 0;
	}
	RES_DIR = argv[1] + string("/");

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
	// Set Char Callback
	glfwSetCharCallback(window, character_callback);
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
