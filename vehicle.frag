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

// lightPerFrag.fp
// This fragment shader takes the various interpolated vector information from the previous stage
// and computes the lighting per fragment. This gives a very smooth look to the lighting regardless
// of the triangle count of the mesh used. The code here is much the same as the vertex stage of
// the lightPerVert.vp example source-file.

uniform vec4 light_pos;

varying vec3 normal;
varying float d;
varying vec3 lightDir;
varying vec3 halfvector;

varying vec4 ambientProduct;
varying vec4 diffuseProduct;
varying vec4 specularProduct;

varying vec2 st;
uniform sampler2D texMap;

const float constantAttn = 0.0;
const float linearAttn = 0.0;
const float quadraticAttn = 0.05;

const float shininess = 64;

void directionalLight(	in vec3 normal,
						inout vec4 ambient,
						inout vec4 diffuse,
						inout vec4 specular)
{
	vec3 VP;
	vec3 HV;
	float NdotVP;
	float NdotHV;
	float pf;	
	
	VP = normalize(light_pos.xyz);
	HV = normalize(halfvector);

	NdotVP = max(0.0, dot(normal, VP));
	NdotHV = max(0.0, dot(normal, HV));
	pf;
	
	if (NdotVP == 0.0)
		pf = 0.0;
	else
		pf = pow(NdotHV, shininess);
	
	ambient += ambientProduct;
	diffuse += diffuseProduct * NdotVP;
	specular += specularProduct * pf;
}

void pointLight(in vec3 normal,
				inout vec4 ambient,
				inout vec4 diffuse,
				inout vec4 specular)
{
	vec3 VP;
	vec3 HV;
	float NdotVP;
	float NdotHV;
	float pf;
	float attenuation;
	
	VP = normalize(lightDir);
	HV = normalize(halfvector);
	
	NdotVP = max(0.0, dot(normal, VP));
	NdotHV = max(0.0, dot(normal, HV));
	
	if (NdotVP == 0.0)
		pf = 0.0;
	else
		pf = pow(NdotHV, shininess);
	
	attenuation = 1.0 / (constantAttn + linearAttn * d + quadraticAttn * d * d);
		
	ambient += ambientProduct * attenuation;
	diffuse += diffuseProduct * NdotVP * attenuation;
	specular += specularProduct * pf * attenuation;
}
				

void main(void) {
	vec4 ambient = vec4(0.0);
	vec4 diffuse = vec4(0.0);
	vec4 specular = vec4(0.0);

	vec3 N = normalize(normal);
	
	float blendFactor = 0.5+0.5*N.y;
	vec4 groundC = vec4(0.2,0.0,0.5,1.0);
	vec4 skyC = vec4(0.1,0.2,0.3,1.0);
	
	vec4 hemisphereC = mix( groundC, skyC, blendFactor );
	
	if (light_pos.w == 0.0)
		directionalLight(N, ambient, diffuse, specular);
	else
		pointLight(N, ambient, diffuse, specular);
	
	gl_FragColor = hemisphereC+(ambient + diffuse + specular)+texture2D(texMap, st); 
}