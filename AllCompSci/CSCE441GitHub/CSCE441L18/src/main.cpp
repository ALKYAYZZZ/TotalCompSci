#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"
#include "GLSL.h"
#include "Program.h"
#include "MatrixStack.h"
#include "Particle.h"
#include "Texture.h"

using namespace std;
using namespace Eigen;

bool keyToggles[256] = {false}; // only for English keyboards!

GLFWwindow *window; // Main application window
string RESOURCE_DIR = ""; // Where the resources are loaded from

shared_ptr<Program> progSimple;
shared_ptr<Program> prog;
shared_ptr<Camera> camera;
vector< shared_ptr<Particle> > particles;
shared_ptr<Texture> texture;
double t, h, e2;
vector<Eigen::Vector3d> allForce;

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
	int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	if(state == GLFW_PRESS) {
		camera->mouseMoved(xmouse, ymouse);
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
		bool ctrl  = mods & GLFW_MOD_CONTROL;
		bool alt   = mods & GLFW_MOD_ALT;
		camera->mouseClicked(xmouse, ymouse, shift, ctrl, alt);
	}
}

static void initGL()
{
	GLSL::checkVersion();
	
	// Set background color
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	// Enable z-buffer test
	glEnable(GL_DEPTH_TEST);
	// Enable alpha blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	progSimple = make_shared<Program>();
	progSimple->setShaderNames(RESOURCE_DIR + "simple_vert.glsl", RESOURCE_DIR + "simple_frag.glsl");
	progSimple->setVerbose(false); // Set this to true when debugging.
	progSimple->init();
	progSimple->addUniform("P");
	progSimple->addUniform("MV");
	
	prog = make_shared<Program>();
	prog->setVerbose(true); // Set this to true when debugging.
	prog->setShaderNames(RESOURCE_DIR + "particle_vert.glsl", RESOURCE_DIR + "particle_frag.glsl");
	prog->init();
	prog->addUniform("P");
	prog->addUniform("MV");
	prog->addAttribute("aPos");
	prog->addAttribute("aTex");
	prog->addUniform("radius");
	prog->addUniform("alphaTexture");
	prog->addUniform("color");
	
	texture = make_shared<Texture>();
	texture->setFilename(RESOURCE_DIR + "alpha.jpg");
	texture->init();
	texture->setUnit(0);
	
	camera = make_shared<Camera>();
	
	// Initialize OpenGL for particles.
	for(auto p : particles) {
		p->init();
	}
	
	// If there were any OpenGL errors, this will print something.
	// You can intersperse this line in your code to find the exact location
	// of your OpenGL error.
	GLSL::checkError(GET_FILE_LINE);
}

// Sort particles by their z values in camera space
class ParticleSorter {
public:
	bool operator()(size_t i0, size_t i1) const
	{
		// Particle positions in world space
		const Vector3d &x0 = particles[i0]->getPosition();
		const Vector3d &x1 = particles[i1]->getPosition();
		// Particle positions in camera space
		float z0 = V.row(2) * Vector4f(x0(0), x0(1), x0(2), 1.0f);
		float z1 = V.row(2) * Vector4f(x1(0), x1(1), x1(2), 1.0f);
		return z0 < z1;
	}
	
	void setViewMatrix(glm::mat4 V2)
	{
		for(int i = 0; i < 4; ++i) {
			for(int j = 0; j < 4; ++j) {
				V(i,j) = V2[j][i]; // indexing is different in Eigen and glm
			}
		}
	}
	
	Matrix4f V; // current view matrix
};
ParticleSorter sorter;

// http://stackoverflow.com/questions/1577475/c-sorting-and-keeping-track-of-indexes
template <typename T>
vector<size_t> sortIndices(const vector<T> &v) {
	// initialize original index locations
	vector<size_t> idx(v.size());
	for (size_t i = 0; i != idx.size(); ++i) idx[i] = i;
	// sort indexes based on comparing values in v
	sort(idx.begin(), idx.end(), sorter);
	return idx;
}

void renderGL()
{
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
	
	// Apply camera transforms
	P->pushMatrix();
	camera->applyProjectionMatrix(P);
	MV->pushMatrix();
	camera->applyViewMatrix(MV);
	// Set view matrix for the sorter
	sorter.setViewMatrix(MV->topMatrix());
	
	// Draw particles
	prog->bind();
	texture->bind(prog->getUniform("alphaTexture"));
	glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, glm::value_ptr(P->topMatrix()));
	// Sort particles by Z for transparency rendering.
	// Since we don't want to modify the contents of the vector, we compute the
	// sorted indices and traverse the particles in this sorted order.
	for(auto i : sortIndices(particles)) {
		particles[i]->draw(prog, MV);
	}
	texture->unbind();
	prog->unbind();
	
	//////////////////////////////////////////////////////
	// Cleanup
	//////////////////////////////////////////////////////
	
	// Pop stacks
	MV->popMatrix();
	P->popMatrix();
	
	GLSL::checkError(GET_FILE_LINE);
}

void saveParticles(const char *filename)
{
	ofstream out(filename);
	if(!out.good()) {
		cout << "Could not open " << filename << endl;
		return;
	}
	
	// 1st line:
	// <n> <h> <e2>
	out << particles.size() << " " << h << " " << " " << e2 << endl;

	// Rest of the lines:
	// <mass> <position> <velocity> <color> <radius>
	
	for (size_t i = 0; i < particles.size(); i++) {
		out << particles[i]->getMass() << " " << particles[i]->getPosition().x() << " " << particles[i]->getPosition().y() << " " << particles[i]->getPosition().z() << " ";
		out << particles[i]->getVelocity().x() << " " << particles[i]->getVelocity().y() << " " << particles[i]->getVelocity().z() << " " << particles[i]->getColor().x() << " ";
		out << particles[i]->getColor().y() << " " << particles[i]->getColor().z() << " " << particles[i]->getRadius() << endl;
	}
	
	out.close();
	cout << "Wrote galaxy to " << filename << endl;
}

void loadParticles(const char *filename)
{
	ifstream in;
	cout << RESOURCE_DIR + filename << endl;
	in.open(RESOURCE_DIR + filename);
	if(!in.good()) {
		cout << "Cannot read " << filename << endl;
		return;
	}

	// 1st line:
	// <n> <h> <e2>
	int n;
	in >> n;
	in >> h;
	in >> e2;

	// Rest of the lines:
	// <mass> <position> <velocity> <color> <radius>

	while (1) {
		if (in.eof()) {
			break;
		}
		Eigen::Vector3d x, v;
		Eigen::Vector3f c;
		float r;
		double m;
		in >> m;
		in >> x.x();
		in >> x.y();
		in >> x.z();
		in >> v.x();
		in >> v.y();
		in >> v.z();
		in >> c.x();
		in >> c.y();
		in >> c.z();
		in >> r;
		shared_ptr<Particle> p = make_shared<Particle>();
		p->setMass(m);
		p->setColor(c);
		p->setPosition(x);
		p->setVelocity(v);
		p->setRadius(r);
		particles.push_back(p);

	}
	


	//
	// IMPLEMENT ME
	//

	in.close();
	cout << "Loaded galaxy from " << filename << endl;
}

void createParticles()
{
	srand(0);
	t = 0.0;
	h = 1.0;
	e2 = 1e-4;

	//
	// REMOVE BELOW AND IMPLEMENT ME
	//
	shared_ptr<Particle> p = make_shared<Particle>();
	p->setMass(1e-3);
	p->setPosition(Eigen::Vector3d(0.0, 0.0, 0.0));
	p->setVelocity(Eigen::Vector3d(0.0, 0.0, 0.0));
	particles.push_back(p);
	shared_ptr<Particle> p2 = make_shared<Particle>();
	p2->setMass(1e-6);
	p2->setPosition(Eigen::Vector3d(1.0, 0.0, 0.0));
	double r = 1.0;
	double a = 1.0;
	double y = sqrt(p->getMass() * ((2.0 / r) - (1.0 / a)));
	p2->setVelocity(Eigen::Vector3d(0.0, y, 0.0));
	particles.push_back(p2);
}

void stepParticles()
{
	if (!allForce.empty()) {
		allForce.clear();
	}
	for (size_t i = 0; i < particles.size(); i++) {
		Eigen::Vector3d total = Eigen::Vector3d(0.0,0.0,0.0);
		for (size_t j = 0; j < particles.size(); j++) {
			if (i != j) {
				Eigen::Vector3d temp;
				Eigen::Vector3d posDiff = particles[j]->getPosition() - particles[i]->getPosition();
				temp = ((particles[i]->getMass() * particles[j]->getMass()) / pow(pow(posDiff.norm(), 2) + e2, 1.5)) * posDiff;
				total += temp;
			}
		}
		allForce.push_back(total);
	}
	for (size_t i = 0; i < particles.size(); i++) {
		Eigen::Vector3d newvel = particles[i]->getVelocity() + ((h / particles[i]->getMass()) * allForce[i]);
		Eigen::Vector3d newpos = particles[i]->getPosition() + (h * newvel);
		particles[i]->setPosition(newpos);
		particles[i]->setVelocity(newvel);
	}
	//
	// IMPLEMENT ME
	//
	t += h;
}

int main(int argc, char **argv)
{
	if(argc != 2 && argc != 3) {
		// Wrong number of arguments
		cout << "Usage: Lab09 <RESOURCE_DIR> <(OPTIONAL) INPUT FILE>" << endl;
		cout << "   or: Lab09 <#steps>       <(OPTIONAL) INPUT FILE>" << endl;
		exit(0);
	}
	// Create the particles...
	if(argc == 2) {
		// ... without input file
		createParticles();
	} else {
		// ... with input file
		RESOURCE_DIR = "../resources" + string("/");
		loadParticles(argv[2]);
	}
	// Try parsing `steps`
	int steps;
	if(sscanf(argv[1], "%i", &steps)) {
		// Success!
		cout << "Running without OpenGL for " << steps << " steps" << endl;
		// Run without OpenGL
		for(int k = 0; k < steps; ++k) {
			stepParticles();
		}
		saveParticles("output.txt");
	} else {
		// `steps` could not be parsed
		cout << "Running with OpenGL" << endl;
		// Run with OpenGL until the window is closed
		//RESOURCE_DIR = argv[1] + string("/");
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
		initGL();
		// Loop until the user closes the window.
		while(!glfwWindowShouldClose(window)) {
			// Step simulation.
			stepParticles();
			// Render scene.
			renderGL();
			// Swap front and back buffers.
			glfwSwapBuffers(window);
			// Poll for and process events.
			glfwPollEvents();
		}
		// Quit program.
		saveParticles("output.txt");
		glfwDestroyWindow(window);
		glfwTerminate();
	}
	cout << "Elapsed time: " << (t*3.261539827498732e6) << " years" << endl;
	return 0;
}
