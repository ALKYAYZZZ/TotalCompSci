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

#include "GLSL.h"
#include "Program.h"
#include "MatrixStack.h"

using namespace std;

GLFWwindow *window; // Main application window
string RESOURCE_DIR = ""; // Where the resources are loaded from
shared_ptr<Program> prog;

bool keyToggles[256] = {false}; // only for English keyboards!
glm::vec2 cameraRotations(0, 0);
glm::vec2 mousePrev(-1, -1);

// Control points
vector<glm::vec3> cps;

enum SplineType
{
	CATMULL_ROM = 0,
	BASIS,
	SPLINE_TYPE_COUNT
};

SplineType type = CATMULL_ROM;

glm::mat4 Bcr, Bb;

vector<pair<float,float> > usTable;

void buildTable();

static void error_callback(int error, const char *description)
{
	cerr << description << endl;
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	
	if(action == GLFW_PRESS) {
		switch(key) {
			case GLFW_KEY_S:
				type = (SplineType)((type + 1) % SPLINE_TYPE_COUNT);
				buildTable();
				break;
			case GLFW_KEY_C:
				cps.clear();
				buildTable();
				break;
			case GLFW_KEY_R:
				cps.clear();
				int n = 8;
				for(int i = 0; i < n; ++i) {
					float alpha = i / (n - 1.0f);
					float angle = 2.0f * M_PI * alpha;
					float radius = cos(2.0f * angle);
					glm::vec3 cp;
					cp.x = radius * cos(angle);
					cp.y = radius * sin(angle);
					cp.z = (1.0f - alpha)*(-0.5) + alpha*0.5;
					cps.push_back(cp);
				}
				buildTable();
				break;
		}
	}
}

static void char_callback(GLFWwindow *window, unsigned int key)
{
	keyToggles[key] = !keyToggles[key];
}

static void cursor_position_callback(GLFWwindow* window, double xmouse, double ymouse)
{
	if(mousePrev.x >= 0) {
		glm::vec2 mouseCurr(xmouse, ymouse);
		cameraRotations += 0.01f * (mouseCurr - mousePrev);
		mousePrev = mouseCurr;
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
		if(mods & GLFW_MOD_SHIFT) {
			// Insert a new control point
			// Convert from window coord to world coord assuming that we're
			// using an orthgraphic projection from -1 to 1.
			float aspect = (float)width/height;
			glm::vec4 x;
			x[0] = 2.0f * ((xmouse / width) - 0.5f)* aspect;
			x[1] = 2.0f * (((height - ymouse) / height) - 0.5f);
			x[2] = 0.0f;
			x[3] = 1.0f;
			// Build the current modelview matrix.
			auto MV = make_shared<MatrixStack>();
			MV->rotate(cameraRotations[1], glm::vec3(1, 0, 0));
			MV->rotate(cameraRotations[0], glm::vec3(0, 1, 0));
			// Since the modelview matrix transforms from world to eye coords,
			// we want to invert to go from eye to world.
			x = glm::inverse(MV->topMatrix()) * x;
			cps.push_back(glm::vec3(x));
			buildTable();
		} else {
			mousePrev.x = xmouse;
			mousePrev.y = ymouse;
		}
	} else {
		mousePrev[0] = -1;
		mousePrev[1] = -1;
	}
}

static void init()
{
	GLSL::checkVersion();
	
	// Set background color
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	// Enable z-buffer test
	glEnable(GL_DEPTH_TEST);
	
	// Initialize the GLSL program.
	prog = make_shared<Program>();
	prog->setVerbose(true);
	prog->setShaderNames(RESOURCE_DIR + "simple_vert.glsl", RESOURCE_DIR + "simple_frag.glsl");
	prog->init();
	prog->addUniform("P");
	prog->addUniform("MV");
	prog->setVerbose(false);
	
	// Initialize time.
	glfwSetTime(0.0);
	
	keyToggles[(unsigned)'l'] = true;
	
	Bcr[0] = glm::vec4( 0.0f,  2.0f,  0.0f,  0.0f);
	Bcr[1] = glm::vec4(-1.0f,  0.0f,  1.0f,  0.0f);
	Bcr[2] = glm::vec4( 2.0f, -5.0f,  4.0f, -1.0f);
	Bcr[3] = glm::vec4(-1.0f,  3.0f, -3.0f,  1.0f);
	Bcr *= 0.5;
	
	Bb[0] = glm::vec4( 1.0f,  4.0f,  1.0f,  0.0f);
	Bb[1] = glm::vec4(-3.0f,  0.0f,  3.0f,  0.0f);
	Bb[2] = glm::vec4( 3.0f, -6.0f,  3.0f,  0.0f);
	Bb[3] = glm::vec4(-1.0f,  3.0f, -3.0f,  1.0f);
	Bb /= 6.0f;

	// If there were any OpenGL errors, this will print something.
	// You can intersperse this line in your code to find the exact location
	// of your OpenGL error.
	GLSL::checkError(GET_FILE_LINE);
}

void buildTable()
{
	usTable.clear();
	int ncps = (int)cps.size();
	glm::mat4 B = (type == CATMULL_ROM ? Bcr : Bb);

	float u = 0.0f;
	float s = 0.0f;
	float prevu = 0.0f;
	int count = -1;
	glm::vec3 puaprev = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 Gk;
	for (int i = 0; i <= (5 * (ncps - 3)); i++) {
		if (i % 5 == 0 && (i != (5 * (ncps - 3)))) {
			count++;
			for (int j = 0; j < 4; ++j) {
				Gk[j] = glm::vec4(cps[count + j], 0.0f);
			}
		}
		float tempu = u - count;
		glm::vec4 u_vec = glm::vec4(1.0f, tempu, tempu*tempu, tempu*tempu*tempu);
		glm::vec3 pub = Gk * (B * u_vec);
		if (i == 0) {
			s = 0.0f;
		}
		else {
			s += glm::length(pub - puaprev);
		}
		usTable.push_back(make_pair(u, s));
		cerr << "U value: " << u << " ";
		cerr << "S Value: " << s << endl;
		puaprev = pub;
		u = u + .2;
	}
	
}

float s2u(float s)
{
	// INSERT CODE HERE
	float s0, s1, u0, u1;
	for (int i = 0; i < usTable.size(); i++) {
		if (usTable[i].second > s) {
			s1 = usTable[i].second;
			s0 = usTable[(i - 1)].second;
			u1 = usTable[i].first;
			u0 = usTable[(i - 1)].first;
			break;
		}
	}
	float alpha = (s - s0) / (s1 - s0);
	float u_final = ((1 - alpha) * u0) + (alpha * u1);
	return u_final;
}

void render()
{
	// Update time.
	//double t = glfwGetTime();
	
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
	
	double aspect = (double)width/height;
	P->multMatrix(glm::ortho(-aspect, aspect, -1.0, 1.0, -2.0, 2.0));
	MV->rotate(cameraRotations.y, glm::vec3(1, 0, 0));
	MV->rotate(cameraRotations.x, glm::vec3(0, 1, 0));
	
	// Bind the program
	prog->bind();
	glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, glm::value_ptr(P->topMatrix()));
	glUniformMatrix4fv(prog->getUniform("MV"), 1, GL_FALSE, glm::value_ptr(MV->topMatrix()));
	
	// Draw control points
	int ncps = (int)cps.size();
	glPointSize(5.0f);
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_POINTS);
	for(int i = 0; i < ncps; ++i) {
		glVertex3f(cps[i].x, cps[i].y, cps[i].z);
	}
	glEnd();
	glLineWidth(1.0f);
	if(keyToggles[(unsigned)'l']) {
		glColor3f(1.0f, 0.5f, 0.5f);
		glBegin(GL_LINE_STRIP);
		for(int i = 0; i < ncps; ++i) {
			glVertex3f(cps[i].x, cps[i].y, cps[i].z);
		}
		glEnd();
	}
	
	if(ncps >= 4) {
		// Draw spline
		glm::mat4 B = (type == CATMULL_ROM ? Bcr : Bb);
		glLineWidth(3.0f);
		for(int k = 0; k < ncps - 3; ++k) {
			glm::mat4 Gk;
			for(int i = 0; i < 4; ++i) {
				Gk[i] = glm::vec4(cps[k+i], 0.0f);
			}
			int n = 32; // curve discretization
			glBegin(GL_LINE_STRIP);
			if(k % 2 == 0) {
				// Even segment color
				glColor3f(0.0f, 1.0f, 0.0f);
			} else {
				// Odd segment color
				glColor3f(0.0f, 0.0f, 1.0f);
			}
			for(int i = 0; i < n; ++i) {
				// u goes from 0 to 1 within this segment
				float u = i / (n - 1.0f);
				// Compute spline point at u
				glm::vec4 uVec(1.0f, u, u*u, u*u*u);
				glm::vec3 P(Gk * (B * uVec));
				glVertex3fv(&P[0]);
			}
			glEnd();
		}
		
		// Draw equally spaced points on the spline curve
		if(keyToggles[(unsigned)'a'] && !usTable.empty()) {
			float ds = 0.2;
			glColor3f(1.0f, 0.0f, 0.0f);
			glPointSize(10.0f);
			glBegin(GL_POINTS);
			float smax = usTable.back().second; // spline length
			for(float s = 0.0f; s < smax; s += ds) {
				// Convert from s to (concatenated) u
				float uu = s2u(s);
				// Convert from concatenated u to the usual u between 0 and 1.
				float kfloat;
				float u = std::modf(uu, &kfloat);
				// k is the index of the starting control point
				int k = (int)std::floor(kfloat);
				// Compute spline point at u
				glm::mat4 Gk;
				for(int i = 0; i < 4; ++i) {
					Gk[i] = glm::vec4(cps[k+i], 0.0f);
				}
				glm::vec4 uVec(1.0f, u, u*u, u*u*u);
				glm::vec3 P(Gk * (B * uVec));
				glVertex3fv(&P[0]);
			}
			glEnd();
		}
	}

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
