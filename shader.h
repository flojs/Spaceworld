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

/**
 * shader.h
 * Creates a shader from two source files, vertex and fragment. Displays debug info for errors in
 * creation. The useful part is the programHandle that is set in construction.
 */

#ifndef SHADER_H
#define SHADER_H

class Shader {
public:

	// The handle to the program object is what you use to interact with your shader program,
	// such as setting the program current or getting the locations of uniforms and attributes.
	// Handle equal to 0 for error; positive, non-zero integer for okay reference
	unsigned int programHandle;

	/**
	 * Generates a new program from the given source files and stores the handle in programHandle
	 * @param vertexSourceFile, file path to the Vertex shader source file
	 * @param fragmentSourceFile, file path to the Fragment shader source file
	 */
	Shader(const char* vertexSourceFile, const char* fragmentSourceFile);

	/**
	 * Get a uniform handle for the given label from this program
	 * @param uniformLable, null terminated string with the name of uniform in the shader program
	 */
	int getUniformHandle(const char *uniformLabel);

private:

	/**
	 * Loads a shader from a source text file generating a shader source of type shaderType
	 * @param sourceFilename, file path to the shader source file
	 * @param shaderType, one of GL_VERTEX_SHADER or GL_FRAGMENT_SHADER
	 * @return handle to the shader program or 0 for error
	 */
	unsigned int loadShader(const char *sourceFilename, unsigned int shaderType);

	/**
	 * Generates a shader program from the given vertex and fragment shader source files and sets
	 * the programHandle to the shader programs handle
	 * @param vertFilename, vertex shader source
	 * @param fragFilename, fragment shader source
	 */
	void makeShaderProgram(const char *vertFilename, const char *fragFilename);

};

#endif
