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

// lightPerFrag.vp
// This is an example of a modified Blinn-Phong lighting model. Two types of lights are shown,
// a directional light and a point/positional light. This example computes the lighting properties
// in both the vertex and fragment shaders. This is an example of a 'per-fragment' lighting model.

uniform mat4 projection_matrix;
uniform mat4 modelview_matrix;
uniform mat3 normal_matrix;
uniform vec4 light_pos;         // Light position in eye-space
uniform vec4 light_ambient;     // Light ambient RGBA values
uniform vec4 light_diffuse;     // Light diffuse RGBA values  
uniform vec4 light_specular;    // Light specular RGBA values

attribute vec3 a_vertex;
attribute vec3 a_normal;
attribute vec4 a_colour_ambient;
attribute vec4 a_colour_diffuse;
attribute vec4 a_colour_specular;
attribute vec2 a_tex_coord;


varying vec3 normal;
varying vec4 c;
varying float d;			// Distance from light to surface
varying vec3 lightDir;		// Light direction for point lights
varying vec3 halfvector;
varying vec4 ambientProduct;
varying vec4 diffuseProduct;
varying vec4 specularProduct;
varying vec2 st;
varying vec4 eyeSpacePos;


void main(void) {

    ambientProduct = a_colour_ambient * light_ambient;
    diffuseProduct = a_colour_diffuse * light_diffuse;
    specularProduct = a_colour_specular * light_specular;
	st = a_tex_coord;

	vec4 ecPos = modelview_matrix * vec4(a_vertex, 1.0);
	vec3 ecPos3 = (vec3(ecPos)) / ecPos.w;
	vec3 eye = -normalize(ecPos.xyz);
	c = a_colour_diffuse;
	normal = normalize(normal_matrix * a_normal);
	
	lightDir = light_pos.xyz - ecPos3;
	d = length(lightDir);
	lightDir = normalize(lightDir);
	
	// Different light types have different half vectors
	if (light_pos.w == 1.0) {
		halfvector = normalize( lightDir + eye );
	} else {
		halfvector = normalize( normalize(light_pos.xyz) + eye );
	}

	eyeSpacePos = ecPos;
	gl_Position = projection_matrix * ecPos;
}