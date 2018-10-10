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

#include "Camera.h"
#include "GLSL.h"
#include "MatrixStack.h"
#include "Program.h"
#include "Shape.h"
#include "Material.h"
#include "Light.h"

using namespace std;

GLFWwindow *window; // Main application window
string RESOURCE_DIR = "./"; // Where the resources are loaded from

shared_ptr<Camera> camera;
shared_ptr<Program> prog;
shared_ptr<Program> prog1;
shared_ptr<Shape> shape;

//Create Materials
vector<Material> material_list;
Material choice_1 = Material(glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.8f, 0.7f, 0.7f), glm::vec3(1.0f, 0.9f, 0.8f), 200.0f, true);
Material choice_2 = Material(glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), 200.0f, false);
Material choice_3 = Material(glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 0.0f), 10.0f, false);

//Create Lights
vector<Light*> light_list;
Light * choice_1_l = new Light(glm::vec4(1.0f, 1.0f, 1.0f, 0.8f), true);
Light * choice_2_l = new Light(glm::vec4(-1.0f, 1.0f, 1.0f, 0.2f), false);

bool selected_sil = true;

bool keyToggles[256] = {false}; // only for English keyboards!

// This function is called when a GLFW error occurs
static void error_callback(int error, const char *description)
{
	cerr << description << endl;
}

// This function is called when a key is pressed
static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}

// This function is called when the mouse is clicked
static void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
	// Get the current mouse position.
	double xmouse, ymouse;
	glfwGetCursorPos(window, &xmouse, &ymouse);
	// Get current window size.
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	if(action == GLFW_PRESS) {
		bool shift = (mods & GLFW_MOD_SHIFT) != 0;
		bool ctrl  = (mods & GLFW_MOD_CONTROL) != 0;
		bool alt   = (mods & GLFW_MOD_ALT) != 0;
		camera->mouseClicked((float)xmouse, (float)ymouse, shift, ctrl, alt);
	}
}

// This function is called when the mouse moves
static void cursor_position_callback(GLFWwindow* window, double xmouse, double ymouse)
{
	int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	if(state == GLFW_PRESS) {
		camera->mouseMoved((float)xmouse, (float)ymouse);
	}
}

static void char_callback(GLFWwindow *window, unsigned int key)
{
	//If m then shift material down
	if (key == 'm') {
		for (size_t i = 0; i < material_list.size(); i++) {
			if (material_list[i].is_selected() == true) {
				cerr << i << endl;
				
				if (((int)i - 1) < 0) {
					material_list[i].unselect();
					material_list[2].select();
					break;
				}
				else {
					material_list[i].unselect();
					material_list[i - 1].select();
					break;
				}
			}
		}
	}
	//if M move material up
	else if (key == 'M') {
		for (size_t i = 0; i < material_list.size(); i++) {
			if (material_list[i].is_selected() == true) {
				if ((i + 1) > material_list.size() - 1) {
					material_list[i].unselect();
					material_list[0].select();
					break;
				}
				else {
					material_list[i].unselect();
					material_list[i + 1].select();
					break;
				}
			}
		}
	}
	//l switches to other light source
	if (key == 'l') {
		for (size_t i = 0; i < light_list.size(); i++) {
			if (light_list[i]->is_selected() == true) {
				cerr << i << endl;

				if (((int)i - 1) < 0) {
					light_list[i]->unselect();
					light_list[1]->select();
					break;
				}
				else {
					light_list[i]->unselect();
					light_list[i - 1]->select();
					break;
				}
			}
		}
	}
	//L switches to other light source
	else if (key == 'L') {
		for (size_t i = 0; i < light_list.size(); i++) {
			if (light_list[i]->is_selected() == true) {
				if ((i + 1) > light_list.size() - 1) {
					light_list[i]->unselect();
					light_list[0]->select();
					break;
				}
				else {
					light_list[i]->unselect();
					light_list[i + 1]->select();
					break;
				}
			}
		}
	}
	//X moves selected light source int the +X direction
	else if (key == 'X') {
		for (size_t i = 0; i < light_list.size(); i++) {
			if (light_list[i]->is_selected() == true) {
				light_list[i]->increment_x();
				break;
			}
		}
	}
	//x moves selected light source int the -X direction
	else if (key == 'x') {
		for (size_t i = 0; i < light_list.size(); i++) {
			if (light_list[i]->is_selected() == true) {
				light_list[i]->decrement_x();
				break;
			}
		}
	}
	//Y moves selected light source int the +Y direction
	else if (key == 'Y') {
		for (size_t i = 0; i < light_list.size(); i++) {
			if (light_list[i]->is_selected() == true) {
				light_list[i]->increment_y();
				break;
			}
		}
	}
	//y moves selected light source int the -Y direction
	else if (key == 'y') {
		for (size_t i = 0; i < light_list.size(); i++) {
			if (light_list[i]->is_selected() == true) {
				light_list[i]->decrement_y();
				break;
			}
		}
	}
	//s switches to other mode
	else if (key == 's') {
		if (selected_sil == true) {
			selected_sil = false;
		}
		else {
			selected_sil = true;
		}
	}
	//S switches to other mode
	else if (key == 'S') {
		if (selected_sil == true) {
			selected_sil = false;
		}
		else {
			selected_sil = true;
		}
	}
}

// If the window is resized, capture the new size and reset the viewport
static void resize_callback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// This function is called once to initialize the scene and OpenGL
static void init()
{
	// Initialize time.
	glfwSetTime(0.0);
	
	// Set background color.
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	// Enable z-buffer test.
	glEnable(GL_DEPTH_TEST);

	//Make program with phong shader fragment info
	prog = make_shared<Program>();
	prog->setShaderNames(RESOURCE_DIR + "vert.glsl", RESOURCE_DIR + "frag.glsl");
	prog->setVerbose(true);
	prog->init();
	prog->addAttribute("aPos");
	prog->addAttribute("aNor");
	prog->addUniform("MV");
	prog->addUniform("P");
	prog->addUniform("lightPos1");
	prog->addUniform("lightPos2");
	prog->addUniform("ka");
	prog->addUniform("kd");
	prog->addUniform("ks");
	prog->addUniform("s");
	prog->setVerbose(false);

	//Make program with silhouette shader fragment info
	prog1 = make_shared<Program>();
	prog1->setShaderNames(RESOURCE_DIR + "vert.glsl", RESOURCE_DIR + "frag_sil.glsl");
	prog1->setVerbose(true);
	prog1->init();
	prog1->addAttribute("aPos");
	prog1->addAttribute("aNor");
	prog1->addUniform("MV");
	prog1->addUniform("P");
	prog1->addUniform("lightPos1");
	prog1->addUniform("lightPos2");
	prog1->addUniform("ka");
	prog1->addUniform("kd");
	prog1->addUniform("ks");
	prog1->addUniform("s");
	prog1->setVerbose(false);

	
	camera = make_shared<Camera>();
	camera->setInitDistance(2.0f);
	
	shape = make_shared<Shape>();
	shape->loadMesh(RESOURCE_DIR + "bunny.obj");
	shape->fitToUnitBox();
	shape->init();
	
	GLSL::checkError(GET_FILE_LINE);
}

// This function is called every frame to draw the scene.
static void render()
{
	// Clear framebuffer.
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
	
	// Get current frame buffer size.
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	camera->setAspect((float)width/(float)height);
	
	// Matrix stacks
	auto P = make_shared<MatrixStack>();
	auto MV = make_shared<MatrixStack>();
	
	// Apply camera transforms
	P->pushMatrix();
	camera->applyProjectionMatrix(P);
	MV->pushMatrix();
	camera->applyViewMatrix(MV);
	
	int chosen_mat, chosen_light;
	for (size_t i = 0; i < material_list.size(); i++) {
		if (material_list[i].is_selected() == true) {
			chosen_mat = (int)i;
		}
	}

	for (size_t i = 0; i <light_list.size(); i++) {
		if (light_list[i]->is_selected() == true) {
			chosen_light = (int)i;
		}
	}

	//Prog is for the Phong shader info
	if (selected_sil == true) {
		prog->bind(selected_sil);
		glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, glm::value_ptr(P->topMatrix()));
		glUniformMatrix4fv(prog->getUniform("MV"), 1, GL_FALSE, glm::value_ptr(MV->topMatrix()));
		glUniform4f(prog->getUniform("lightPos1"), choice_1_l->get_light_pos().x, choice_1_l->get_light_pos().y, choice_1_l->get_light_pos().z, choice_1_l->get_light_pos().w);
		glUniform4f(prog->getUniform("lightPos2"), choice_2_l->get_light_pos().x, choice_2_l->get_light_pos().y, choice_2_l->get_light_pos().z, choice_2_l->get_light_pos().w);
		glUniform3f(prog->getUniform("ka"), material_list[chosen_mat].get_ka().x, material_list[chosen_mat].get_ka().y, material_list[chosen_mat].get_ka().z);
		glUniform3f(prog->getUniform("kd"), material_list[chosen_mat].get_kd().x, material_list[chosen_mat].get_kd().y, material_list[chosen_mat].get_kd().z);
		glUniform3f(prog->getUniform("ks"), material_list[chosen_mat].get_ks().x, material_list[chosen_mat].get_ks().y, material_list[chosen_mat].get_ks().z);
		glUniform1f(prog->getUniform("s"), material_list[chosen_mat].get_s());
		shape->draw(prog);
		prog->unbind();
	}
	//Prog1 is for the sihouette info
	else {
		prog1->bind(selected_sil);
		glUniformMatrix4fv(prog1->getUniform("P"), 1, GL_FALSE, glm::value_ptr(P->topMatrix()));
		glUniformMatrix4fv(prog1->getUniform("MV"), 1, GL_FALSE, glm::value_ptr(MV->topMatrix()));
		glUniform4f(prog1->getUniform("lightPos1"), choice_1_l->get_light_pos().x, choice_1_l->get_light_pos().y, choice_1_l->get_light_pos().z, choice_1_l->get_light_pos().w);
		glUniform4f(prog1->getUniform("lightPos2"), choice_2_l->get_light_pos().x, choice_2_l->get_light_pos().y, choice_2_l->get_light_pos().z, choice_2_l->get_light_pos().w);
		glUniform3f(prog1->getUniform("ka"), material_list[chosen_mat].get_ka().x, material_list[chosen_mat].get_ka().y, material_list[chosen_mat].get_ka().z);
		glUniform3f(prog1->getUniform("kd"), material_list[chosen_mat].get_kd().x, material_list[chosen_mat].get_kd().y, material_list[chosen_mat].get_kd().z);
		glUniform3f(prog1->getUniform("ks"), material_list[chosen_mat].get_ks().x, material_list[chosen_mat].get_ks().y, material_list[chosen_mat].get_ks().z);
		glUniform1f(prog1->getUniform("s"), material_list[chosen_mat].get_s());
		shape->draw(prog1);
		prog1->unbind();
	}

	

	
	
	MV->popMatrix();
	P->popMatrix();
	
	GLSL::checkError(GET_FILE_LINE);
}

int main(int argc, char **argv)
{
	//Push back materials
	material_list.push_back(choice_1);
	material_list.push_back(choice_2);
	material_list.push_back(choice_3);
	//Push back lights
	light_list.push_back(choice_1_l);
	light_list.push_back(choice_2_l);
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
	GLSL::checkVersion();
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
	// Set the window resize call back.
	glfwSetFramebufferSizeCallback(window, resize_callback);
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
