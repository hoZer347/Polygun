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
#include <memory>

#include <vector>
#include <string>
#include <map>

#include "Geometry.h"
#include "ObjManager.h"
#include "Camera.h"

#define SHADER_DIRECTORY "../Polygun/"

class App {
public:
	App();
	~App();

	// Shader stuff
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
	//

	// Starts the game loop
	void init();

	// Pumps the geometry into the buffers
	void pump();

		// --Inputs functions-- //
	// Does input polls
	void do_inputs();

	// Callback Functions
	static void CursorCallback(GLFWwindow*, double, double);
	static void KeyPrsCallback(GLFWwindow*, int, int, int, int);
	static void MouseBCallback(GLFWwindow*, int, int, int);
	static void ResizeCallback(GLFWwindow*, int, int);
	static void ScrollCallback(GLFWwindow*, double, double);
		// -------------------- //

	Camera cam;
	ObjManager obj;

private:
	GLuint shader=0, vao=0, vbo=0, ibo=0, do_frame=0, mvpID=0;
	GLint vtxID=0, nrmID=0, clrID=0, mdlID=0, viwID=0, prjID=0;
	GLuint shading=0;
	GLFWwindow* window;

	bool dyn_light = false;
	float sensitivity = 4;
};

