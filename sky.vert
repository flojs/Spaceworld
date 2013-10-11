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

#version 120

// manyAttributes.vp
// This shader passes on colour values to be interpolated by the fixed functionality
// 

uniform mat4 projection_matrix;
uniform mat4 modelview_matrix;
uniform mat3 normal_matrix;

attribute vec3 a_vertex;
attribute vec4 a_colour_ambient;	
attribute vec3 a_normal;

// This colour attribute will be sent out per-vertex and the fixed-functionality
// interpolates the colour (smooths the colour between) with connected vertices in the triangle
varying vec4 colour;

varying vec3 texCoord;


void main(void) {
	
	colour = a_colour_ambient;	// We simply pass the colour along to the next stage

    vec3 N = normalize(normal_matrix * a_normal);    

	gl_Position = projection_matrix * modelview_matrix * vec4(a_vertex, 1.0); //ensure the skybox behaves according to the transformations put on the modelview_Matrix
	texCoord = a_vertex;

}