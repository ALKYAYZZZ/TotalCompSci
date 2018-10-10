#include "Program.h"

#include <iostream>
#include <cassert>

#include "GLSL.h"

using namespace std;

Program::Program() :
	vShaderName(""),
	fShaderName(""),
	pid1(0),
	pid0(0),
	verbose(true)
{
	
}

Program::~Program()
{
	
}

void Program::setShaderNames(const string &v, const string &f)
{
	vShaderName = v;
	fShaderName = f;
}

bool Program::init()
{
	GLint rc;
	
	// Create shader handles
	GLuint VS = glCreateShader(GL_VERTEX_SHADER);
	GLuint FS = glCreateShader(GL_FRAGMENT_SHADER);
	
	// Read shader sources
	const char *vshader = GLSL::textFileRead(vShaderName.c_str());
	const char *fshader = GLSL::textFileRead(fShaderName.c_str());
	glShaderSource(VS, 1, &vshader, NULL);
	glShaderSource(FS, 1, &fshader, NULL);
	
	// Compile vertex shader
	glCompileShader(VS);
	glGetShaderiv(VS, GL_COMPILE_STATUS, &rc);
	if(!rc) {
		if(isVerbose()) {
			GLSL::printShaderInfoLog(VS);
			cout << "Error compiling vertex shader " << vShaderName << endl;
		}
		return false;
	}
	
	// Compile fragment shader
	glCompileShader(FS);
	glGetShaderiv(FS, GL_COMPILE_STATUS, &rc);
	if(!rc) {
		if(isVerbose()) {
			GLSL::printShaderInfoLog(FS);
			cout << "Error compiling fragment shader " << fShaderName << endl;
		}
		return false;
	}
	

		// Create the program and link
		pid0 = glCreateProgram();
		glAttachShader(pid0, VS);
		glAttachShader(pid0, FS);
		glLinkProgram(pid0);
		glGetProgramiv(pid0, GL_LINK_STATUS, &rc);
		if (!rc) {
			if (isVerbose()) {
				GLSL::printProgramInfoLog(pid0);
				cout << "Error linking shaders " << vShaderName << " and " << fShaderName << endl;
			}
			return false;
		}

	
	GLSL::checkError(GET_FILE_LINE);
	return true;
}

void Program::bind(bool selected_1)
{
		glUseProgram(pid0);
	
}

void Program::unbind()
{
	glUseProgram(0);
}

void Program::addAttribute(const string &name)
{

		attributes0[name] = glGetAttribLocation(pid0, name.c_str());
	
}

void Program::addUniform(const string &name)
{

		uniforms0[name] = glGetUniformLocation(pid0, name.c_str());

}

GLint Program::getAttribute(const string &name) const
{

		map<string, GLint>::const_iterator attribute = attributes0.find(name.c_str());
		if (attribute == attributes0.end()) {
			if (isVerbose()) {
				cout << name << " is not an attribute variable" << endl;
			}
			return -1;
		}
		return attribute->second;

}

GLint Program::getUniform(const string &name) const
{

		map<string, GLint>::const_iterator uniform = uniforms0.find(name.c_str());
		if (uniform == uniforms0.end()) {
			if (isVerbose()) {
				cout << name << " is not a uniform variable" << endl;
			}
			return -1;
		}
		return uniform->second;

}
