#pragma once

#define GLM_FORCE_RADIANS


#include <glew.h>
#include <glut.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc/quaternion.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>

#include <fstream>
#include <sstream>

#include <iostream>

#include <vector>
#include <string>

#include "Cube.h"
#include "Camera.h"
#include "Object.h"

#define SHADER_DIRECTORY "C:/Users/liam/source/repos/Polygun/Polygun/"

class App {
public:
	App();
	~App();

	std::string LF(std::string filename) {
		const char* char_file = filename.c_str();
		std::ifstream f;
		f.open(char_file);
		if (f.fail()) {
			throw(std::ios_base::failure(std::string("Error opening file ") + std::string(filename)));
		}

		std::string content;
		std::string line;
		while (std::getline(f, line)) {
			content += line + "\n";
		}

		f.close();

		return content;
	}
	GLuint LS(std::string shaderName) {
		// Create a shader from vertex program source code
		GLuint vs = glCreateShader(GL_VERTEX_SHADER);

		//Read in the shaders into strings
		std::string fragment_shader = LF(std::string(SHADER_DIRECTORY) + shaderName + ".frag");
		std::string vertex_shader = LF(std::string(SHADER_DIRECTORY) + shaderName + ".vert");

		//Compile the vertex shader from the source string
		const char* const_vertex_shader = vertex_shader.c_str();
		glShaderSource(vs, 1, &const_vertex_shader, NULL);
		glCompileShader(vs);

		// Check if shader compiled successfully
		GLint status;
		glGetShaderiv(vs, GL_COMPILE_STATUS, &status);
		if (status != GL_TRUE) {
			char buffer[512];
			glGetShaderInfoLog(vs, 512, NULL, buffer);
			throw(std::ios_base::failure(std::string("Error compiling vertex shader: ") + std::string(buffer)));
		}

		//Compile the fragment shader from the source string
		const char* const_fragment_shader = fragment_shader.c_str();
		GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fs, 1, &const_fragment_shader, NULL);
		glCompileShader(fs);

		// Check if shader compiled successfully
		glGetShaderiv(fs, GL_COMPILE_STATUS, &status);
		if (status != GL_TRUE) {
			char buffer[512];
			glGetShaderInfoLog(fs, 512, NULL, buffer);
			throw(std::ios_base::failure(std::string("Error compiling fragment shader: ") + std::string(buffer)));
		}

		// Create a shader program linking both vertex and fragment shaders together
		GLuint shader = glCreateProgram();
		glAttachShader(shader, vs);
		glAttachShader(shader, fs);
		glLinkProgram(shader);

		// Check if shaders were linked successfully
		glGetProgramiv(shader, GL_LINK_STATUS, &status);
		if (status != GL_TRUE) {
			char buffer[512];
			glGetShaderInfoLog(shader, 512, NULL, buffer);
			throw(std::ios_base::failure(std::string("Error linking shaders: ") + std::string(buffer)));
		}

		// Delete memory used by shaders, since they were already compiled
		// and linked
		glDeleteShader(vs);
		glDeleteShader(fs);

		return shader;
	}

	void init();
	void pump(std::vector<GLfloat>, GLenum);

		// INPUT FUNCTIONS //
	// Does input polls
	void do_inputs();

	// Callback Functions
	static void CursorCallback(GLFWwindow*, double, double);
	static void KeyPrsCallback(GLFWwindow*, int, int, int, int);
	static void MouseBCallback(GLFWwindow*, int, int, int);
	static void ResizeCallback(GLFWwindow*, int, int);
	static void ScrollCallback(GLFWwindow*, double, double);
		// -------------- //

private:
	GLuint shader=0, mvpID=0, clrID=0, vertexbuffer=0, VertexArrayID=0;
	GLFWwindow* window;

	std::vector<GLfloat> vertices;
	std::vector<GLfloat> frame;

	std::vector<Object*> objects;

	double mx=0, my=0;

	Camera cam;
};

