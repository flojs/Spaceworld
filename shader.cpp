/*********************************************************
 Version 1.000
 
 Code provided by Michael Hemsley and Anthony Dick
 for the course 
 COMP SCI 3014/7090 Computer Graphics
 School of Computer Science
 University of Adelaide
 
 Permission is granted for anyone to copy, use, modify, or distribute this
 program and accompanying programs and documents for any purpose, provided
 this copyright notice is retained and prominently displayed, along with
 a note saying that the original programs are available from the aforementioned 
 course web page. 
 
 The programs and documents are distributed without any warranty, express or
 implied.  As the programs were written for research purposes only, they have
 not been tested to the degree that would be advisable in any important
 application.  All use of these programs is entirely at the user's own risk.
 *********************************************************/


#include <iostream>
#include <fstream>
#include <stdio.h>
#include <iomanip>
#include <stdlib.h>

#ifdef MacOSX
#include "GLee.h"
#include <GLUT/glut.h>
#else
#include "GLee.h"
#include <GL/glut.h>
#endif

#include "shader.h"

#define SOURCE_LINE_BUFFER_LEN 	1024
#define SOURCE_MAX_LINES		256


Shader::Shader(const char* vertexSourceFile, const char* fragmentSourceFile) {

	makeShaderProgram(vertexSourceFile, fragmentSourceFile);

}


int Shader::getUniformHandle(const char *uniformLabel) {

	int uniformHandle = glGetUniformLocation(programHandle, uniformLabel);
	if (uniformHandle == -1)
		std::cout << "Uniform: " << uniformLabel << " was not an active uniform label\n";

	return uniformHandle;
}


unsigned int Shader::loadShader(const char *sourceFilename, unsigned int shaderType) {

	// Load the shader source code from file
	std::ifstream in;
	in.open(sourceFilename, std::ifstream::in);

	char *line;					// Single line
	const char *sourceCode[SOURCE_MAX_LINES];

	int numLines = 0;
	if (in.is_open()) {
		// Read in all lines of source
		while (in.good()) {
			line = new char[SOURCE_LINE_BUFFER_LEN];		// New buffer
			in.getline (line, SOURCE_LINE_BUFFER_LEN);	// Fill with one line
			int numChars = in.gcount();

			// Add newlines in
			if (numChars != 0 && line[numChars-1] == '\0') {
				line[numChars-1] = '\n';
				line[numChars] = '\0';
			}

			// Copy line
			sourceCode[numLines] = line;

			numLines++;
		}
	} else {
		std::cout << "Failed to open source file: " << sourceFilename << "\n";
		return 0;	// Return error
	}

	in.close();

	// Create a handle to shader
	unsigned int shaderHandle = glCreateShader(shaderType);

	// Load the source
	glShaderSource(shaderHandle, numLines, sourceCode, NULL);

	// Try to compile
	glCompileShader(shaderHandle);

	// Check compiled okay
	int res;
	glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &res);

	// If error
	if (res != GL_TRUE) {

		// Show the log
		int len;
		char log[1024];
		glGetShaderInfoLog(shaderHandle, 1024, &len, log);
		std::cout << "\n" << log << "\n";

		// Display source and line numbers
		for (int i=0; i<numLines; i++)
			std::cout << std::setw(3) << i << ":\t" << sourceCode[i];
		std::cout << "\n\n";

		glDeleteShader(shaderHandle);
	}

	// Clean up
	for (int i=0; i<numLines; i++)
		delete [] sourceCode[i];

	// Result
	if (res != GL_TRUE)
		return 0;				// Return error
	else
		return shaderHandle;	// Return handle
}


void Shader::makeShaderProgram(const char *vertFilename, const char *fragFilename) {

	// New program
	programHandle = glCreateProgram();

	unsigned int vertexShader = loadShader(vertFilename, GL_VERTEX_SHADER);
	unsigned int fragmentShader = loadShader(fragFilename, GL_FRAGMENT_SHADER);

	if (vertexShader == 0 || fragmentShader == 0) {
		programHandle = 0;	// Return error
		return;
	}

	// Attach them to the program
	glAttachShader(programHandle, vertexShader);
	glAttachShader(programHandle, fragmentShader);

	// Link creates the program from the attached shaders
	glLinkProgram(programHandle);

	// Check linked okay
	int res;
	glGetProgramiv(programHandle, GL_LINK_STATUS, &res);

	// If error
	if (res != GL_TRUE) {
		// Show the log
		int len;
		char log[1024];
		glGetProgramInfoLog(programHandle, 1024, &len, log);
		std::cout << log << "\n";

		programHandle = 0;	// Return error
		return;
	}
}

