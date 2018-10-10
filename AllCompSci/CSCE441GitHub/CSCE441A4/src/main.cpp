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
#include "Object.h"
#include "Freelook.h"
#include "Texture.h"

using namespace std;

GLFWwindow *window; // Main application window
string RESOURCE_DIR = "./"; // Where the resources are loaded from

//Create all Shared Pointers and Vectors For Object Data
shared_ptr<Freelook> camera;
shared_ptr<Program> prog;
shared_ptr<Program> prog1;
shared_ptr<Shape> shape;
shared_ptr<Shape> shape1;
shared_ptr<Shape> shape2;
shared_ptr<Texture> texture0;
vector<float> texBuf;
vector<float> posBuf;
vector<unsigned int> indBuf;
map<string, GLuint> bufIDs;
int indCount;
glm::mat3 T1;


//Create Materials
vector<Material> material_list;
Material choice_1 = Material(glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.8f, 0.7f, 0.7f), glm::vec3(1.0f, 0.9f, 0.8f), 200.0f, true);
Material choice_2 = Material(glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), 200.0f, false);
Material choice_3 = Material(glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 0.0f), 10.0f, false);

//Create Lights
vector<Light*> light_list;
Light * choice_1_l = new Light(glm::vec4(-10.0f, 3.0f, 40.0f, 0.8f), true);
Light * choice_2_l = new Light(glm::vec4(-1.0f, 1.0f, 1.0f, 0.2f), false);

vector<Object*> obj_list;

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
	if (state == GLFW_PRESS) {
		camera->mouseMoved((float)xmouse, (float)ymouse, window);
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
	//c switches to other mode
	else if (key == 'c') {
		if (selected_sil == true) {
			selected_sil = false;
		}
		else {
			selected_sil = true;
		}
	}
	//C switches to other mode
	else if (key == 'C') {
		if (selected_sil == true) {
			selected_sil = false;
		}
		else {
			selected_sil = true;
		}
	}
	//Move Camera in Forward Direction
	else if (key == 'w') {
		camera->moveW();
	}
	//Move Camera in Left Direction
	else if (key == 'a') {
		camera->moveA();
	}
	//Move Camera in Back Direction
	else if (key == 's') {
		camera->moveS();
	}
	//Move Camera in Right Direction
	else if (key == 'd') {
		camera->moveD();
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
	prog->addAttribute("aTex");
	prog->addUniform("MV");
	prog->addUniform("P");
	prog->addUniform("V");
	prog->addUniform("lightPos1");
	prog->addUniform("lightPos2");
	prog->addUniform("ka");
	prog->addUniform("kd");
	prog->addUniform("ks");
	prog->addUniform("s");
	prog->addUniform("t");
	prog->addUniform("T1");
	prog->addUniform("texture0");
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

	
	camera = make_shared<Freelook>();
	camera->setInitDistance(2.0f);
	
	//Initalize First Shape
	shape = make_shared<Shape>();
	shape->loadMesh(RESOURCE_DIR + "bunny.obj");
	shape->fitToUnitBox();
	shape->init();

	//Initialize Second Shape
	shape1 = make_shared<Shape>();
	shape1->loadMesh(RESOURCE_DIR + "teapot.obj");
	shape1->fitToUnitBox();
	shape1->init();

	//Initialize Sun Object
	shape2 = make_shared<Shape>();
	shape2->loadMesh(RESOURCE_DIR + "sphere.obj");
	shape2->fitToUnitBox();
	shape2->init();

	//Initialize Texture and set to Repeat Mode
	texture0 = make_shared<Texture>();
	texture0->setFilename(RESOURCE_DIR + "dancefloor_02.gif");
	texture0->init();
	texture0->setUnit(0);
	texture0->setWrapModes(GL_REPEAT, GL_REPEAT);

	//
	// Texture matrix
	//
	T1[0][0] = 2.0f;
	T1[1][1] = 2.0f;

	//
	// Initialize geometry
	//
	// We need to fill in the position buffer, normal buffer, the texcoord
	// buffer, and the index buffer.
	// 0
	posBuf.push_back(100.0f);
	posBuf.push_back(-1.0f);
	posBuf.push_back(100.0f);
	texBuf.push_back(0.0f);
	texBuf.push_back(0.0f);
	// 1
	posBuf.push_back(-100.0f);
	posBuf.push_back(-1.0f);
	posBuf.push_back(100.0f);
	texBuf.push_back(1.0f);
	texBuf.push_back(0.0f);
	// 2
	posBuf.push_back(100.0f);
	posBuf.push_back(-1.0f);
	posBuf.push_back(-100.0f);
	texBuf.push_back(0.0f);
	texBuf.push_back(1.0f);
	// 3
	posBuf.push_back(-100.0f);
	posBuf.push_back(-1.0f);
	posBuf.push_back(-100.0f);
	texBuf.push_back(1.0f);
	texBuf.push_back(1.0f);
	// Index
	indBuf.push_back(0);
	indBuf.push_back(1);
	indBuf.push_back(2);
	indBuf.push_back(3);
	indBuf.push_back(2);
	indBuf.push_back(1);
	indCount = (int)indBuf.size();

	// Generate 3 buffer IDs and put them in the bufIDs map.
	GLuint tmp[3];
	glGenBuffers(3, tmp);
	bufIDs["bPos"] = tmp[0];
	bufIDs["bTex"] = tmp[1];
	bufIDs["bInd"] = tmp[2];

	//Bind all Buffers to Hold all Texture and Position Data
	glBindBuffer(GL_ARRAY_BUFFER, bufIDs["bPos"]);
	glBufferData(GL_ARRAY_BUFFER, posBuf.size() * sizeof(float), &posBuf[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, bufIDs["bTex"]);
	glBufferData(GL_ARRAY_BUFFER, texBuf.size() * sizeof(float), &texBuf[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIDs["bInd"]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indBuf.size() * sizeof(unsigned int), &indBuf[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	//Initialize a List Containing 200 different Colored Objects at Different Angles
	for (int i = 0; i < 200; i++) {
		Object * temp = new Object(rand() % 2);
		temp->chooseCoords();
		temp->setDiffComp();
		obj_list.push_back(temp);
	}
	
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
	auto V = make_shared<MatrixStack>();
	
	float t = (float)glfwGetTime();

	// Apply camera transforms
	P->pushMatrix();
	camera->applyProjectionMatrix(P);
	V->pushMatrix();
	camera->applyViewMatrix(V);
	MV->pushMatrix();
	camera->applyViewMatrix(MV);

	//Remaining Material Switching Loop From A3
	int chosen_mat, chosen_light;
	for (size_t i = 0; i < material_list.size(); i++) {
		if (material_list[i].is_selected() == true) {
			chosen_mat = (int)i;
		}
	}
	//Remaining Light Switching Loop From A3
	for (size_t i = 0; i < light_list.size(); i++) {
		if (light_list[i]->is_selected() == true) {
			chosen_light = (int)i;
		}
	}
	//Implement Textured Ground using the vectors containing the position, texture and normal info.
	prog->bind(selected_sil);
	texture0->bind(prog->getUniform("texture0"));
	glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, glm::value_ptr(P->topMatrix()));
	glUniformMatrix4fv(prog->getUniform("MV"), 1, GL_FALSE, glm::value_ptr(MV->topMatrix()));
	glUniformMatrix3fv(prog->getUniform("T1"), 1, GL_FALSE, glm::value_ptr(T1));
	glUniformMatrix4fv(prog->getUniform("V"), 1, GL_FALSE, glm::value_ptr(V->topMatrix()));
	glEnableVertexAttribArray(prog->getAttribute("aPos"));
	glBindBuffer(GL_ARRAY_BUFFER, bufIDs["bPos"]);
	glVertexAttribPointer(prog->getAttribute("aPos"), 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
	glEnableVertexAttribArray(prog->getAttribute("aTex"));
	glBindBuffer(GL_ARRAY_BUFFER, bufIDs["bTex"]);
	glVertexAttribPointer(prog->getAttribute("aTex"), 2, GL_FLOAT, GL_FALSE, 0, (void *)0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIDs["bInd"]);
	glDrawElements(GL_TRIANGLES, indCount, GL_UNSIGNED_INT, (void *)0);
	texture0->unbind();
	prog->unbind();



	//Translate and Initialize the Sun Object and its Light Source
	MV->pushMatrix();
		//To Change With Time, a sin and cos function are used for the sun to orbit
		MV->translate(-10.0f - (60.0f * sin(t/2.0f)), 3.0f, 40.0f - (60.0f * cos(t/2.0f)));
		prog->bind(selected_sil);
		glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, glm::value_ptr(P->topMatrix()));
		glUniformMatrix4fv(prog->getUniform("MV"), 1, GL_FALSE, glm::value_ptr(MV->topMatrix()));
		glUniformMatrix4fv(prog->getUniform("V"), 1, GL_FALSE, glm::value_ptr(V->topMatrix()));
		glUniform4f(prog->getUniform("lightPos1"), choice_1_l->get_light_pos().x , choice_1_l->get_light_pos().y, choice_1_l->get_light_pos().z, choice_1_l->get_light_pos().w);
		glUniform3f(prog->getUniform("ka"), 1.0f, 1.0f, 0.0f);
		glUniform3f(prog->getUniform("kd"), 1.0f, 1.0f, 0.0f);
		glUniform3f(prog->getUniform("ks"), 1.0f, 1.0f, 0.0f);
		glUniform1f(prog->getUniform("s"), 10);
		glUniform1f(prog->getUniform("t"), t);
		shape2->draw(prog);
		prog->unbind();
	MV->popMatrix();

	//For all the initialized Objects, Randomly Distribute them around the environment and intialize their
	//Fragment and Vert Shaders
	for (int h = 0; h < 200; h++) {
		MV->pushMatrix();
		Object * temp = obj_list[h];
		float yval;
		if (temp->getDesc() == 1) {
			yval = temp->getY();
		}
		else {
			yval = -0.25f;
		}
		MV->translate(temp->getX(), yval, temp->getZ());
		MV->rotate(temp->getRotZ() / (t * 100.0f), 0.0f, 1.0f, 0.0f);
		MV->scale( 3.0f, 3.0f, 3.0f);

		//Prog is for the Phong shader info
		if (selected_sil == true) {
			prog->bind(selected_sil);
			glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, glm::value_ptr(P->topMatrix()));
			glUniformMatrix4fv(prog->getUniform("MV"), 1, GL_FALSE, glm::value_ptr(MV->topMatrix()));
			glUniformMatrix4fv(prog->getUniform("V"), 1, GL_FALSE, glm::value_ptr(V->topMatrix()));
			glUniform4f(prog->getUniform("lightPos1"), choice_1_l->get_light_pos().x, choice_1_l->get_light_pos().y, choice_1_l->get_light_pos().z, choice_1_l->get_light_pos().w);
			glUniform4f(prog->getUniform("lightPos2"), choice_2_l->get_light_pos().x, choice_2_l->get_light_pos().y, choice_2_l->get_light_pos().z, choice_2_l->get_light_pos().w);
			glUniform3f(prog->getUniform("ka"), material_list[chosen_mat].get_ka().x, material_list[chosen_mat].get_ka().y, material_list[chosen_mat].get_ka().z);
			glUniform3f(prog->getUniform("kd"), obj_list[h]->getDiffComp().x, obj_list[h]->getDiffComp().y, obj_list[h]->getDiffComp().z);
			glUniform3f(prog->getUniform("ks"), material_list[chosen_mat].get_ks().x, material_list[chosen_mat].get_ks().y, material_list[chosen_mat].get_ks().z);
			glUniform1f(prog->getUniform("s"), material_list[chosen_mat].get_s());
			//Choose which Shape must be drawn by using random initialized value.
			if (temp->getDesc() == 1){
				shape->draw(prog);
			}
			else {
				shape1->draw(prog);
			}
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
	}
	
	MV->popMatrix();
	V->popMatrix();
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
