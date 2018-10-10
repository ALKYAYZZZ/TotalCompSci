#include <iostream>
#include <vector>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define EIGEN_DONT_ALIGN_STATICALLY
#include <Eigen/Dense>

#include "GLSL.h"
#include "Program.h"
#include "MatrixStack.h"

using namespace std;

GLFWwindow *window; // Main application window
string RESOURCE_DIR = ""; // Where the resources are loaded from
shared_ptr<Program> prog;

bool keyToggles[256] = {false}; // only for English keyboards!
glm::vec2 mouse;

Eigen::Vector4f coeffs0;
Eigen::Vector4f coeffs1;
Eigen::MatrixXf A(8, 8);
Eigen::VectorXf b(8);
Eigen::VectorXf c(8);
float xmid;

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
}

static void cursor_position_callback(GLFWwindow* window, double xmouse, double ymouse)
{
	// Convert from window coords to world coords
	// (Assumes orthographic projection)
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	glm::vec4 p;
	// Inverse of viewing transform
	p.x = xmouse / (float)width;
	p.y = (height - ymouse) / (float)height;
	p.x = 2.0f * (p.x - 0.5f);
	p.y = 2.0f * (p.y - 0.5f);
	p.z = 0.0f;
	p.w = 1.0f;
	// Inverse of model-view-projection transform
	auto P = make_shared<MatrixStack>();
	auto MV = make_shared<MatrixStack>();
	float aspect = (float)width/height;
	float s = 0.6f;
	P->multMatrix(glm::ortho(-s*aspect, s*aspect, -s, s));
	MV->translate(glm::vec3(-0.5, -0.5, 0.0));
	p = glm::inverse(P->topMatrix() * MV->topMatrix()) * p;
	mouse = glm::vec2(p);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	// Not used for this lab
}

static void init()
{
	GLSL::checkVersion();
	
	// Set background color
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	// Enable z-buffer test
	glEnable(GL_DEPTH_TEST);
	
	// Initialize the GLSL program.
	prog = make_shared<Program>();
	prog->setVerbose(false); // Set this to true when debugging.
	prog->setShaderNames(RESOURCE_DIR + "simple_vert.glsl", RESOURCE_DIR + "simple_frag.glsl");
	prog->init();
	prog->addUniform("P");
	prog->addUniform("MV");
	
	//
	// Compute the coefficients here
	//
	xmid = 0.4f;
	//coeffs0 << -2.0f, 3.0f, 0.0f, 0.0f;
	//coeffs1 << -2.0f, 3.0f, 0.0f, 0.0f;
	
	Eigen::Vector4f btemp;
	Eigen::Matrix4f Atemp;

	btemp << 0.2f, 0.0f, 1.0f, 0.0f;
	Atemp << pow(0.5f, 3), pow(0.5f, 2), 0.5f, 1.0f,
		3 * pow(0.5f, 2), 2 * 0.5f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		3 * 1.0f, 2 * 1.0f, 1.0f, 0.0f;
	Eigen::VectorXf ctemp = Atemp.colPivHouseholderQr().solve(btemp);


	A << 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		pow(0.4f, 3), pow(0.4f, 2), 0.4f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		3 * pow(0.4f, 2), 2 * 0.4f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, pow(0.5f, 3), pow(0.5f, 2), 0.5f, 1.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 3 * pow(0.5f, 2), 2 * 0.5f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 3 * 1.0f, 2 * 1.0f, 1.0f, 0.0f;
	float f1at4 = ((ctemp(0) * pow(0.4f, 3)) + (ctemp(1) * pow(0.4f, 2)) + (ctemp(2) * 0.4f) + (ctemp(3) *1.0f));
	float f1primeat4 = ((3 * ctemp(0) * pow(0.4f, 2)) + (2 * ctemp(1) * 0.4f) + (ctemp(2)));
	b << 0.0f, 0.0f, f1at4, f1primeat4, 0.2f, 0.0f, 1.0f, 0.0f;
	Eigen::VectorXf c = A.colPivHouseholderQr().solve(b);
	coeffs0 << c(0), c(1), c(2), c(3);
	coeffs1 << c(4), c(5), c(6), c(7);

	
	// If there were any OpenGL errors, this will print something.
	// You can intersperse this line in your code to find the exact location
	// of your OpenGL error.
	GLSL::checkError(GET_FILE_LINE);
}

void render()
{
	// Get current frame buffer size.
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
	
	// Clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	auto P = make_shared<MatrixStack>();
	auto MV = make_shared<MatrixStack>();
	P->pushMatrix();
	MV->pushMatrix();
	
	float aspect = (float)width/height;
	float s = 0.6f;
	P->multMatrix(glm::ortho(-s*aspect, s*aspect, -s, s));
	MV->translate(glm::vec3(-0.5, -0.5, 0.0));
	
	// Bind the program
	prog->bind();
	glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, glm::value_ptr(P->topMatrix()));
	glUniformMatrix4fv(prog->getUniform("MV"), 1, GL_FALSE, glm::value_ptr(MV->topMatrix()));
	
	// Draw grid
	int gridSize = 5;
	glLineWidth(2.0f);
	glColor3f(0.2f, 0.2f, 0.2f);
	glBegin(GL_LINES);
	for(int i = 1; i < gridSize; ++i) {
		float x = i / (float)gridSize;
		glVertex2f(x, 0.0f);
		glVertex2f(x, 1.0f);
	}
	for(int i = 1; i < gridSize; ++i) {
		float y = i / (float)gridSize;
		glVertex2f(0.0f, y);
		glVertex2f(1.0f, y);
	}
	glEnd();
	glLineWidth(4.0f);
	glColor3f(0.8f, 0.8f, 0.8f);
	glBegin(GL_LINE_LOOP);
	glVertex2f(0.0f, 0.0f);
	glVertex2f(1.0f, 0.0f);
	glVertex2f(1.0f, 1.0f);
	glVertex2f(0.0f, 1.0f);
	glEnd();

	//
	// Draw cubics here
	//
	
	glLineWidth(2.0f);
	glBegin(GL_LINE_STRIP);
	glColor3f(1.0f, 0.5f, 1.0f);
	float tangent = 0;
	float f1 = 0;
	float u = (float)mouse.x;
	float total = 0;
	float totalu = 0;
	float tangent1 = 0;
	vector<float> tangvec;
	if ((float)mouse.x <= xmid) {
		tangent = (coeffs0(0) * 3.0f * pow(u, 2.0f)) + (2.0f * coeffs0(1) * u) + coeffs0(2);
		f1 = (coeffs0(0) * (pow(u, 3.0))) + (coeffs0(1) * (pow(u, 2.0))) + (coeffs0(2) * u) + coeffs0(3);
	}
	else {
		tangent = (coeffs1(0) * 3.0f * pow(u, 2.0f)) + (2.0f * coeffs1(1) * u) + coeffs1(2);
		f1 = (coeffs1(0) * (pow(u, 3.0))) + (coeffs1(1) * (pow(u, 2.0))) + (coeffs1(2) * u) + coeffs1(3);
	}
	float norm1 = sqrt(pow(-.1f, 2.0) + pow(((tangent * -0.1f)), 2.0)) * 10.0f;
	float norm2 = sqrt(pow(.1f, 2.0) + pow(((tangent * 0.1f)), 2.0)) * 10.0f;
	
	glVertex2f((float) -.1f / norm1 + u, ((float)(tangent * -0.1f) / norm1 + f1));
	//glVertex2f((float)u / norm, ((float)f1) / norm);
	glVertex2f((float).1f / norm2 + u, ((float)(tangent * 0.1f) / norm2 + f1) );
	glEnd();

	glLineWidth(2.0f);
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < 100; i++) {
		u = (float)i / 100.0f;
		float f;

		if (u <= xmid) {
			glColor3f(1.0f, 0.5f, 0.4f);
			f = (coeffs0(0) * (pow(u, 3.0))) + (coeffs0(1) * (pow(u, 2.0))) + (coeffs0(2) * u) + coeffs0(3);
		}
		else {
			glColor3f(0.0f, 0.2f, 1.0);
			f = (coeffs1(0) * (pow(u, 3.0))) + (coeffs1(1) * (pow(u, 2.0))) + (coeffs1(2) * u) + coeffs1(3);
		}

		glVertex2f((float)u, (float)f);
	}
	glEnd();

	// Unbind the program
	prog->unbind();

	// Pop matrix stacks.
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
