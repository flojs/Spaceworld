
#include <iostream>
#include <stdlib.h>
#include <iomanip>
#include <stdio.h>
#include <fstream>
#include <math.h>
#include <vector>

#ifdef MacOSX
#include "GLee.h"
#include <GLUT/glut.h>
#else
#include "GLee.h"
#include <GL/glut.h>
#endif

#include "shader.h"
#include "matrix4f.h"
#include "cube.h"
#include "imageloader.h"
#include "scenery.h"
#include "ModelViewer.h"
#include <complex>
typedef std::complex<float>  Complex;

using std::vector;

#define WINDOW_WIDTH 	640
#define WINDOW_HEIGHT 	480
#define TEX_SIZE        64

#define PI 3.14159265

#define CAMERA_ROT_AMOUNT 3.0f	// The amount the camera rotates in degrees each 'step'
#define CAMERA_TRANS_AMOUNT 0.1f // The amount the camera translates
#define VEHICLE_ROT_AMOUNT 1.0f
#define VEHICLE_ACCEL_AMOUNT 0.001f

#define TICK_MILLS 10

#define MAP_X	32				         // size of map along x-axis
#define MAP_Z	32				         // size of map along z-axis
#define MAP_SCALE	1.0f		         // the scale of the terrain map
////////procedural/////////
#define CENTERX -0.5
#define CENTERY 0.0
#define HEIGHT 2.0
#define WIDTH 2.0
#define MAX_ITER 1000

/* N x M array to be generated */

#define N_PROC 32 //512 //should be same as MAP_X ,MAP_Z!!!!
#define M_PROC 32 //512

float height_comp = HEIGHT;          /* size of window in complex plane */
float width_comp = WIDTH;
float cx = CENTERX;             /* center of window in complex plane */
float cy = CENTERY;
int max = MAX_ITER;             /* number of interations per point */

int n_proc = N_PROC;
int m_proc = M_PROC;

GLfloat imageProcedural[N_PROC][M_PROC][3];

/////////////////////////

float terrain[N_PROC][M_PROC][3];		// heightfield terrain data (0-255); 256x256

Image* imageData;
char* generatedImage;	

// Texture handle
unsigned int *textureHandleArr;
unsigned int textureHandle;

// A square
unsigned int groundVaoHandle;
unsigned int skyVaoHandle;
unsigned int terrainVaoHandle;
unsigned int *modelVaoHandle = new unsigned int[10]; //change
GLuint *textures = new GLuint[10]; // Change this to be based on total textures!!!

float* modelvertices;
float* modelnormals;
float* tex_coordinate;

int indexVert = 0;

int textureUniformHandle;
int modelviewUniformHandle;
int normalMatrixUniformHandle;

int numberOfModels;

//size of the world..
float groundX = 30.0f;
float groundZ = 30.0f;

// Our shader program
Shader *program;
Scenery *ground;

Cube *skycube;

Matrix4f cameraMatrix, projection;
Matrix4f groundMatrix, modelMatrix, modelMatrixRot;
// An accumulation of the input for our camera rotation
float cameraRotDelta = 0.0f;
float cameraRotDelta2 = 0.0f;
float cameraTranslation = -5.0f;

float vehicleAngle = 0.0f;
float vehicleSpeed = 0.0f;
float vehicleAccel = 2.0f;
float vehiclePos_x = 0.0f;
float vehiclePos_y = 0.0f;

vector<ModelViewer*> modelVector; 
vector<const char *> modelNameVec; 

float *vertexArray;
float *normalArray;
float *colorArray;
float *texArray;
unsigned int *indicesTer;

inline float Random1() { return float( rand() % 31 + 1 ); }
inline float Random2() { return float( rand() % 5 ); }
inline float Random3() { return float( rand() % 628 ); } //should be 2*PI ..here 2*pi*100 

struct ShaderStruct 
{
	GLuint shaderProgram;
	GLuint projectUniformHandle,
		modelviewUniformHandle,
		normalMatrixUniformHandle,
		textureUniformHandle,
		lightambientUniformHandle,
		lightdiffuseUniformHandle,
		lightspecularUniformHandle,
		lightposUniformHandle;
	GLuint vertLoc,
		normLoc,
		colour_ambientLoc,
		colour_diffuseLoc,
		colour_specularLoc,
		texLoc;
};

struct MeshObject 
{
	ShaderStruct shader;
	GLuint vertexArrayObject;
	unsigned int indices;

	MeshObject(ShaderStruct shader, GLuint vertexArrayObject, unsigned int indices)
		:shader(shader),vertexArrayObject(vertexArrayObject),indices(indices){}

	MeshObject():indices(0){}
};

MeshObject skyboxObject;
MeshObject vehicleObject;
MeshObject groundObject;
MeshObject barrelObject;
MeshObject terrainObject;

ShaderStruct programSkyBox;
ShaderStruct programVehicle;
ShaderStruct programScenery;

/////////////////////////////////////////////
/////////////////////////////////////////////
void proceduralIslandTerrain()
{
	float minHeight = 1000.0f; 
	float maxHeight = 0.0f;
	int numIt = 250;

	for( int a = 0 ; a<numIt; a++)
	{ 
		////To create land at completely random spots..

		//int centerPointX = Random1();
		//int centerPointZ = Random1();

		////..or to make a centered island

		//int radius = Random2();
		//printf("radius: %i \n", radius );
		//float direction = float(Random2());
		//printf("direction: %.2f \n", direction );
		//float distance = float(rand() % (N_PROC/2 - radius));
		//printf("distance: %.2f \n", distance );
		//int centerPointX = N_PROC/2+cos(direction)*distance;
		//int centerPointZ = M_PROC/2+sin(direction)*distance;

		////..or to make land along the edge 

		int radius = Random2();
		//printf("radius: %i \n", radius );
		float direction = float(Random3())/100.0f;
		//printf("direction: %.2f \n", direction );
		float distance = float(rand() % (N_PROC/2) + N_PROC/2.5f ); //Changed here, to force bigger dist 
		//printf("distance: %.2f \n", distance );
		int centerPointX = N_PROC/2+cos(direction)*distance;
		int centerPointZ = M_PROC/2+sin(direction)*distance;
	
		for ( int i = 0; i < N_PROC; i++ )
		{
			for ( int j = 0; j < M_PROC; j++ ) 
			{	//calculate the height
				float heightValue = radius*radius - ( (i - centerPointX)*(i - centerPointX) + (j - centerPointZ)*(j - centerPointZ) );
				//printf("HeightValue: %.2f \n", heightValue );
				//throw away the negative heights
				if(heightValue >= 0.0f) 
				{
					if(heightValue > maxHeight){maxHeight = heightValue;}
					if(heightValue < minHeight){minHeight = heightValue;}
					imageProcedural[i][j][1] = imageProcedural[i][j][1] + heightValue;
				}
				//printf("x: %.2f ,y: %.2f , z: %.2f ", imageProcedural[i][j][0] , imageProcedural[i][j][1], imageProcedural[i][j][2] );
			}
		}
	}
	//printf("minH: %.2f maxH: %.2f \n", minHeight , maxHeight);

	//normalize the height
	for ( int i = 0; i < N_PROC; i++ )
	{
        for ( int j = 0; j < M_PROC; j++ ) 
		{
			float y_norm = (imageProcedural[i][j][1] - minHeight) / (maxHeight - minHeight);
			//printf("y_norm %.2f \n", y_norm);
			imageProcedural[i][j][1] = y_norm;
        }
	}
}


void mandelbrotFunc(){

	for ( int i = 0; i < N_PROC; i++ ){
        for ( int j = 0; j < M_PROC; j++ ) 
		{
            float x = i * ( width_comp / (n_proc - 1) ) + cx - width_comp / 2;
            float y = j * ( height_comp / ( m_proc - 1 ) ) + cy - height_comp / 2;

			Complex c( 0.0, 0.0 );
			Complex p( x, y );

			float  v;
            for ( int k = 0; k < max; k++ ) 
			{
				// compute c = c^2 + p
				c *= c;
				c += p;
				v = norm( c );
				if ( v > 5.0 )
				{
					break;      /* assume not in set if mag > 4..1 */
				}
            }
            // assign gray level to point based on its magnitude */
            if ( v > 1.0 )
			{
                v = 1.0;        /* clamp if > 1 */
			}
            imageProcedural[i][j][0] = v;
            imageProcedural[i][j][1] = 2.0 * sin( v ) - 1.0;
            imageProcedural[i][j][2] = 1.0 - v;
			//printf("x: %.2f ,y: %.2f , z: %.2f ", imageProcedural[i][j][0] , imageProcedural[i][j][1], imageProcedural[i][j][2] );
        }
	}
}

void InitializeTerrain()
{
	//Change the method here, by which to generate the terrain:  
	proceduralIslandTerrain();
	
	//mandelbrotFunc();

	//..Or use a greyscale picture:

	//imageData = loadBMP("terrain.bmp");
	//generatedImage = imageData->pixels;

	// loop through all of the heightfield points, calculating
	// the coordinates for each point
	int incrI = 0;
	for (int z = 0; z < MAP_Z; z++)
	{
		for (int x = 0; x < MAP_X; x++)
		{	
			//generatedImage[incrI*3] = 0.0f;
			//printf("generatedImage[z*MAP_Z+x]: %.2f \n",(float)generatedImage[(z*MAP_Z+x)*3]);
			terrain[x][z][0] = float(x)*MAP_SCALE;				
			terrain[x][z][1] = (float)imageProcedural[x][z][1] *2 ;// *Scalefactor 

			//terrain[x][z][1] = (float)generatedImage[(z*MAP_Z+x)*3]; //Use for greyscale picture instead!!! 

			terrain[x][z][2] = -float(z)*MAP_SCALE;
			incrI += 1;
		}
	}
}

void setupHeightMap()
{
	//printf("vertexArray,: (MAP_Z-1) * (MAP_X-1) *12: %i \n",(MAP_Z-1) * (MAP_X-1) *12);
	//printf("colorArray: (MAP_Z-1) * (MAP_X-1) *16: %i \n",(MAP_Z-1) * (MAP_X-1) *16);
	//printf("texArray: (MAP_Z-1) * (MAP_X-1) *8: %i \n",(MAP_Z-1) * (MAP_X-1) *8);
	//printf("indicesTer: (MAP_Z-1) * (MAP_X-1) *3: %i \n",(MAP_Z-1) * (MAP_X-1) *12);
	vertexArray = new float[(MAP_Z-1) * (MAP_X-1) *12];   
	colorArray = new float[(MAP_Z-1) * (MAP_X-1)*16];
	normalArray = new float[(MAP_Z-1) * (MAP_X-1) *12];
	texArray = new float[(MAP_Z-1) * (MAP_X-1)*8];
	indicesTer = new unsigned int[(MAP_Z-1) * (MAP_X-1) *12];
	int incrV = 0;
	int incrC = 0;
	int incrT = 0;
	float sColor = 1.0f;
	float gColor = 1.0f;
	float waterThres = 0.0f;

	for (int z = 0; z < MAP_Z-1; z++)
	{
		for (int x = 0; x < MAP_X-1; x++)
		{
			// draw vertex 0
			vertexArray[incrV] = terrain[x][z][0];
			vertexArray[incrV+1] = terrain[x][z][1]; //the y coordinate , regulate for best fit . eg. /20.0f !!!!!!!!!!!!!!!!!!!!!!!!
			vertexArray[incrV+2] = terrain[x][z][2];

			if( vertexArray[incrV+1] > waterThres){ sColor = 0.0f; gColor = 1.0f; }// make it green if above or blue otherwise!
			else{ sColor = 1.0f; gColor = 0.0f;}

			colorArray[incrC] = terrain[x][z][1]/255.0f;
			colorArray[incrC+1] = gColor;
			colorArray[incrC+2] = sColor;
			colorArray[incrC+3] = 1.0f;
			
			texArray[incrT] = 0.0f;
			texArray[incrT+1] = 0.0f;

			// draw vertex 1
			vertexArray[incrV+3] = terrain[x+1][z][0];
			vertexArray[incrV+4] = terrain[x+1][z][1];
			vertexArray[incrV+5] = terrain[x+1][z][2];

			if( vertexArray[incrV+4] > waterThres){ sColor = 0.0f; gColor = 1.0f; }// make it green if above or blue otherwise!
			else{ sColor = 1.0f; gColor = 0.0f;}

			colorArray[incrC+4] = terrain[x+1][z][1]/255.0f;
			colorArray[incrC+5] = gColor;
			colorArray[incrC+6] = sColor;
			colorArray[incrC+7] = 1.0f;
			
			texArray[incrT+2] = 1.0f;
			texArray[incrT+3] = 0.0f;

			// draw vertex 2

			vertexArray[incrV+6] = terrain[x][z+1][0];
			vertexArray[incrV+7] = terrain[x][z+1][1];
			vertexArray[incrV+8] = terrain[x][z+1][2];

			if( vertexArray[incrV+7] > waterThres){ sColor = 0.0f; gColor = 1.0f; }// make it green if above or blue otherwise!
			else{ sColor = 1.0f; gColor = 0.0f;}

			colorArray[incrC+8] = terrain[x][z+1][1]/255.0f;
			colorArray[incrC+9] = gColor;
			colorArray[incrC+10] = sColor;
			colorArray[incrC+11] = 1.0f;
			
			texArray[incrT+4] = 0.0f;
			texArray[incrT+5] = 1.0f;

			// draw vertex 3
			vertexArray[incrV+9] = terrain[x+1][z+1][0];
			vertexArray[incrV+10] = terrain[x+1][z+1][1];
			vertexArray[incrV+11] = terrain[x+1][z+1][2];
			int counterTest = 0;
			if( vertexArray[incrV+10] > waterThres){ sColor = 0.0f; gColor = 1.0f; }// make it green if above or blue otherwise!
			else{ sColor = 1.0f; gColor = 0.0f;}

			colorArray[incrC+12] = terrain[x+1][z+1][1]/255.0f;
			colorArray[incrC+13] = gColor;
			colorArray[incrC+14] = sColor;
			colorArray[incrC+15] = 1.0f;
			
			texArray[incrT+6] = 1.0f;
			texArray[incrT+7] = 1.0f;

			//fix to avoid the visibility of the connected Quad strips.  
			if(x == 0 || x == MAP_X-2){ 
				vertexArray[incrV+1] = -3.0f; vertexArray[incrV+4] = -3.0f; vertexArray[incrV+7] = -3.0f;vertexArray[incrV+10] = -3.0f; }

			incrV+= 12;
			incrC+= 16;
			incrT+= 8;

		}
	}
	printf("incr %i \n",incrV);
	for (int a = 0; a<incrV;a++){ 
		indicesTer[a] = a;
		//printf("vertexX: %.2f, vertexY: %.2f, vertexZ: %.2f \n", vertexArray[3*a] , vertexArray[3*a+1], vertexArray[3*a+2] );
		//Todo: calculate Normals.
	}
}



/////////////////////////////////////////////
////////////////////////////////////////////



ShaderStruct loadShader(const char* vertShader, const char* fragShader) {
	ShaderStruct shader;
	Shader *program = new Shader(vertShader, fragShader); 
	shader.shaderProgram  = program->programHandle;

	// get uniform locations
	
	shader.projectUniformHandle = glGetUniformLocation(shader.shaderProgram, "projection_matrix");
	shader.textureUniformHandle = glGetUniformLocation(shader.shaderProgram, "texMap");
	shader.modelviewUniformHandle = glGetUniformLocation(shader.shaderProgram, "modelview_matrix");
    shader.normalMatrixUniformHandle = glGetUniformLocation(shader.shaderProgram, "normal_matrix");
	shader.lightambientUniformHandle = glGetUniformLocation(shader.shaderProgram, "light_ambient");
	shader.lightdiffuseUniformHandle = glGetUniformLocation(shader.shaderProgram, "light_diffuse");
	shader.lightspecularUniformHandle = glGetUniformLocation(shader.shaderProgram, "light_specular");
	shader.lightposUniformHandle = glGetUniformLocation(shader.shaderProgram, "light_pos");

	
	// get attribute locations
	shader.vertLoc = glGetAttribLocation(shader.shaderProgram, "a_vertex");
	shader.normLoc = glGetAttribLocation(shader.shaderProgram, "a_normal");
    shader.colour_ambientLoc = glGetAttribLocation(shader.shaderProgram, "a_colour_ambient");
	shader.colour_diffuseLoc = glGetAttribLocation(shader.shaderProgram, "a_colour_diffuse");
	shader.colour_specularLoc = glGetAttribLocation(shader.shaderProgram, "a_colour_specular");
	shader.texLoc = glGetAttribLocation(shader.shaderProgram, "a_tex_coord");

	return shader;
}

int createVAO(ShaderStruct shader, float *vertices, float *normals, float *tex_coord, float *coloursA, float *coloursD, float *coloursS, unsigned int *indices,
				int vertCount, int normCount, int texCount, int coloCount, int indexCount) {

	printf("Inside createVAOs \n");
	unsigned int vaoHandle;
	glGenVertexArrays(1, &vaoHandle);
	glBindVertexArray(vaoHandle);
    
	unsigned int buffer[7];
	glGenBuffers(7, buffer);

	// Set vertex attribute
	glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertCount, vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(shader.vertLoc);
	glVertexAttribPointer(shader.vertLoc, VALS_PER_VERT, GL_FLOAT, GL_FALSE, 0, 0);

		// Normal attributes
	glBindBuffer(GL_ARRAY_BUFFER, buffer[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*normCount, normals, GL_STATIC_DRAW);
	glEnableVertexAttribArray(shader.normLoc);
	glVertexAttribPointer(shader.normLoc, VALS_PER_NORMAL, GL_FLOAT, GL_FALSE, 0, 0);
	
	// Ambient Colour attributes
	glBindBuffer(GL_ARRAY_BUFFER, buffer[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*coloCount, coloursA, GL_STATIC_DRAW);
	glEnableVertexAttribArray(shader.colour_ambientLoc);
	glVertexAttribPointer(shader.colour_ambientLoc, VALS_PER_COLOUR, GL_FLOAT, GL_FALSE, 0, 0);

    // Diffuse Colour attributes
	glBindBuffer(GL_ARRAY_BUFFER, buffer[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*coloCount, coloursD, GL_STATIC_DRAW);
	glEnableVertexAttribArray(shader.colour_diffuseLoc);
	glVertexAttribPointer(shader.colour_diffuseLoc, VALS_PER_COLOUR, GL_FLOAT, GL_FALSE, 0, 0);

	// Specular Colour attributes
	glBindBuffer(GL_ARRAY_BUFFER, buffer[4]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*coloCount, coloursS, GL_STATIC_DRAW);
	glEnableVertexAttribArray(shader.colour_specularLoc);
	glVertexAttribPointer(shader.colour_specularLoc, VALS_PER_COLOUR, GL_FLOAT, GL_FALSE, 0, 0);

	// Texture attributes
	glBindBuffer(GL_ARRAY_BUFFER, buffer[5]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*texCount, tex_coord, GL_STATIC_DRAW);
	glEnableVertexAttribArray(shader.texLoc);
	glVertexAttribPointer(shader.texLoc, VALS_PER_TEX, GL_FLOAT, GL_FALSE, 0, 0);
	
    // Set element attributes. Notice the change to using GL_ELEMENT_ARRAY_BUFFER
	// We don't attach this to a shader label, instead it controls how rendering is performed
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[6]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*indexCount, indices, GL_STATIC_DRAW);   

	
    // Un-bind
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
    
	printf("out createVAOs \n");
	return vaoHandle;
}


unsigned int texGen(const char *filename, int currentT) 
{
   printf("Inside texGen, name: %s \n", filename );
    Image* image = loadBMP(filename); 
	// A shader program has many texture units, slots in which a texture can be bound, available to
	// it and this function defines which unit we are working with currently
	glActiveTexture(GL_TEXTURE0 + currentT);
	// Sets ogl state to work on this currently empty texture
	// AND associates active texture unit to use this texture we are building
	glBindTexture(GL_TEXTURE_2D, textures[currentT]);
	// TexImage loads the image data to the currently bound texture
	// See ogl spec. for full details of this methods parameters.
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,image->width, image->height,0,GL_RGB,GL_UNSIGNED_BYTE,image->pixels);
	// Linear interpolation filter gives a smoothed out appearance 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// Clamp to edge can help to remove visible seams when lookups are at the edge of the texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    printf("End texGen \n");
	return currentT;
}

void setModelVAOs(int currentMaterial, int currentModel, unsigned int modelIter)
{
	printf("Inside setModelVAOs \n");
	int currVar = 3* modelVector[currentModel]->numVerArray[currentMaterial];
	printf("currVar: %i \n", currVar);

	float materialDiffuse[ currVar*VALS_PER_COLOUR ];
	float materialAmbient[ currVar*VALS_PER_COLOUR ];
	float materialSpecular[ currVar*VALS_PER_COLOUR ];
	unsigned int indices[currVar ];
	
	for(int a = 0; a<currVar ; a++)
	{

		materialDiffuse[4*a] = modelVector[currentModel]->mtl->diff[0];
		materialDiffuse[4*a+1] = modelVector[currentModel]->mtl->diff[1];
		materialDiffuse[4*a+2] = modelVector[currentModel]->mtl->diff[2];
		materialDiffuse[4*a+3] = modelVector[currentModel]->mtl->diff[3];

		materialAmbient[4*a] = modelVector[currentModel]->mtl->amb[0];
		materialAmbient[4*a+1] = modelVector[currentModel]->mtl->amb[1];
		materialAmbient[4*a+2] = modelVector[currentModel]->mtl->amb[2];
		materialAmbient[4*a+3] = modelVector[currentModel]->mtl->amb[3];

		materialSpecular[4*a] = modelVector[currentModel]->mtl->spec[0];
		materialSpecular[4*a+1] = modelVector[currentModel]->mtl->spec[1];
		materialSpecular[4*a+2] = modelVector[currentModel]->mtl->spec[2];
		materialSpecular[4*a+3] = modelVector[currentModel]->mtl->spec[3];

		indices[a] = a;
		//printf("indices: %i\n", indices[a]);
	}

	modelvertices = modelVector[currentModel]->vertexArray;
	modelnormals = modelVector[currentModel]->normalArray;

	tex_coordinate = modelVector[currentModel]->textureArray;

	bool texBool = true;
	if(tex_coordinate == 0)
	{
		printf("no texture \n");
		texBool = false;

	} 

	//printf("modelVector[currentModel]->numVerArray[currentMaterial], currentMaterial: %i , %i \n", modelVector[currentModel]->numVerArray[currentMaterial],currentMaterial);
	
	//Arrayes used to get the values of the current model material only to pass to the createVao function.
	float modelverticesCurr[currVar]; 
	float modelnormalsCurr[currVar];
	float modeltextureCurr[currVar*2];
	
	if (currentMaterial > 0) //sum of prev(s) until sum of prev(s)+size
	{
		int prevVar = indexVert;
		printf("prevVar: %i\n", prevVar);
		for(int a = prevVar ; a < prevVar + currVar; a++) 
		{
			//printf("a, prevVar + currVar: %i , %i \n", a, prevVar + currVar);
			modelverticesCurr[a-prevVar] = modelvertices[a];
			modelnormalsCurr[a-prevVar] = modelnormals[a];
			//if texture..
			if(texBool)
			{
				for(int b=0;b<2;b++)
				{
					modeltextureCurr[2*(a-prevVar)+b] = tex_coordinate[2*a+b];
				}
			}
		}
		indexVert = indexVert + currVar;
	}
	else
	{
		for(int a = 0; a < currVar; a++) 
		{
			modelverticesCurr[a] = modelvertices[a];
			modelnormalsCurr[a] = modelnormals[a];
			if(texBool)
			{
				for(int b=0;b<2;b++)
				{
					modeltextureCurr[2*a+b] = tex_coordinate[2*a+b];
				}
			}
		}
		indexVert = currVar;
	}

	//printf("Number of model vertices * x,y,z: %i\n", modelVector[currentModel]->vertexArrayCount );	
//	for(unsigned int a = 0; a<currVar ; a++)
//	{
//		printf("modelvertices: %.2f \n", modelverticesCurr[a]);
//		printf("modelnormals: %.2f \n", modelnormalsCurr[a]);
//	}
	ShaderStruct shader;
	if( modelIter == 1) {
		shader = programScenery;
	}
	else if (modelIter == 2){
		shader = programVehicle;
	}
	modelVaoHandle[modelIter] = createVAO(shader,modelverticesCurr,modelnormalsCurr,modeltextureCurr,materialAmbient,materialDiffuse,materialSpecular, indices, currVar, currVar,modelVector[currentModel]->numVerArray[currentMaterial]*2,currVar/3*VALS_PER_COLOUR, currVar/3);
										//float *vertices, float *normals, float *tex_coord, float *coloursA, float *coloursD, float *coloursS, unsigned int *indices,int vertCount, int normCount, int texCount, int coloCount, int indexCount			
	printf("modelVaoHandle[modelIter], modelIter: %i , %i\n",modelVaoHandle[modelIter], modelIter);
	printf("End setModelVAOs \n");
}


void setUpLight(ShaderStruct shader)
{
	//Light
    
	float lightambient[4] = { 0.12f, 0.08f, 0.10f, 1.0f };	// ambient light components
    float lightdiffuse[4] = { 1.0f, 1.0f, 1.0f, 1.0f };	// diffuse light components
    float lightspecular[4] = { 1.0f, 1.0f, 1.0f, 1.0f };	// specular light components
	
	glUniform4fv(shader.lightambientUniformHandle, 1, lightambient);	// Set already transformed light position
	glUniform4fv(shader.lightdiffuseUniformHandle, 1, lightdiffuse);	// Set already transformed light position
	glUniform4fv(shader.lightspecularUniformHandle, 1, lightspecular);	// Set already transformed light position
}

unsigned int texCubeMapGen(int num) {

	Image* xpos = loadBMP("xpos.bmp"); 
	Image* ypos = loadBMP("yneg.bmp"); //the y's are shifted to add up!!!!
	Image* zpos = loadBMP("zpos.bmp"); 
	Image* xneg = loadBMP("xneg.bmp"); 
	Image* yneg = loadBMP("ypos.bmp"); 
	Image* zneg = loadBMP("zneg.bmp"); 

	glActiveTexture(GL_TEXTURE0+num);
    // texture set up

    glBindTexture( GL_TEXTURE_CUBE_MAP, textures[0] );
    
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, xpos->width, xpos->height, 0, GL_RGB, GL_UNSIGNED_BYTE, xpos->pixels);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, xneg->width, xneg->height, 0, GL_RGB, GL_UNSIGNED_BYTE, xneg->pixels);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, ypos->width, ypos->height, 0, GL_RGB, GL_UNSIGNED_BYTE, ypos->pixels);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, yneg->width, yneg->height, 0, GL_RGB, GL_UNSIGNED_BYTE, yneg->pixels);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, zpos->width, zpos->height, 0, GL_RGB, GL_UNSIGNED_BYTE, zpos->pixels);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, zneg->width, zneg->height, 0, GL_RGB, GL_UNSIGNED_BYTE, zneg->pixels);
            
    glTexParameterf( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	return num;
    
}

void initTexture(){
	
	glGenTextures(10, textures);
	textureHandleArr = new unsigned int[10];
	
	textureHandleArr[0] = texGen("Water.bmp",0); 
	textureHandleArr[8] = texGen("Water.bmp",8);
	textureHandleArr[9] = texCubeMapGen(9);
	printf("textureHandleArr[0]: %i \n", textureHandleArr[0]);
}

void loadModels() 
{
	printf("Inside load Models \n");
	//add models to the scene!
	modelNameVec.push_back("Barrel02.obj");
	modelNameVec.push_back("Wavecraft2.obj");
	
	numberOfModels = modelNameVec.size();
	printf("numberOfModels: %i \n", numberOfModels);
	unsigned int texincr = 0;

	for(int b=0; b<numberOfModels;b++) //for each model
	{	
		modelVector.push_back(new ModelViewer);
		modelVector[b]->load(modelNameVec[b]);
		for(int a=0;a<modelVector[b]->materialCount;a++) // for each material
		{	
			modelVector[b]->render(a);
			printf("texName: %s \n", modelVector[b]->mtl->texture_filename);
			//printf("texCount: %i \n", modelVector[b]->textureCount);
			//printf("vertexCount: %i \n", modelVector[b]->vertexCount);
			//is there texture or not for the material.
	
			if(modelVector[b]->mtl->texture_filename[0] != '\0')
			{  
				texincr += 1;
				printf("yes tex");
				// set up the texture with this call
				textureHandleArr[texincr] = texGen( modelVector[b]->mtl->texture_filename, texincr); 	
				printf("textureHandleArr[texincr], texincr: %i, %i \n", textureHandleArr[texincr],texincr);
			}
			else
			{
				printf("no tex \n");
			}
			setModelVAOs(a,b,texincr);
		}
	}
	printf("out load Models \n");
}

void drawMeshObject(int quad, Matrix4f projection, Matrix4f modelview, MeshObject meshObject, unsigned int texNum) { 
//void drawMeshObject(Matrix4f &projection, Matrix4f &modelview, MeshObject &meshObject) { 
	glUseProgram(meshObject.shader.shaderProgram);
	
	glUniformMatrix4fv(meshObject.shader.projectUniformHandle, 1, false, projection.m);
	
	glUniformMatrix4fv(meshObject.shader.modelviewUniformHandle, 1, false, modelview.m);

	glUniformMatrix3fv(meshObject.shader.normalMatrixUniformHandle, 1, false, modelview.getNormalMat().m);

	glUniform1i(meshObject.shader.textureUniformHandle, textureHandleArr[texNum] );
	
	glBindVertexArray(meshObject.vertexArrayObject);
	if(quad == 1){
		glDrawElements(GL_QUADS, meshObject.indices, GL_UNSIGNED_INT, 0);
	}
	else if(quad == 2){
		glDrawElements(GL_TRIANGLES, meshObject.indices, GL_UNSIGNED_INT, 0);
	}
	else{
		glDrawElements(GL_TRIANGLE_STRIP, meshObject.indices, GL_UNSIGNED_INT, 0);
	}
}

void drawModel(int currentMaterial,int currentModel, unsigned int iterNumber)
{	

	glBindVertexArray(modelVaoHandle[iterNumber]);
	
	if(currentModel == 0)
	{	
		for(int i=0;i<4;i++)
		{	
			for(int j=0;j<15;j++)
			{
				modelMatrix.copy(cameraMatrix);
				if(i == 0)
				{
					modelMatrix.translate(-groundX/2 , 0.0f , -groundZ/2+2.0f*j);
				}
				else if(i == 1)
				{
					modelMatrix.translate(groundX/2 ,0.0f , -groundZ/2+2.0f*j);
				}
				else if(i == 2)
				{
					modelMatrix.translate(-groundX/2+2.0f*j ,0.0f , -groundZ/2);
				}
				else if(i == 3)
				{
					modelMatrix.translate(-groundX/2+2.0f*j ,0.0f , groundZ/2);
				}
				
				modelMatrix.scale(20.0f, 20.0f, 20.0f);
				modelMatrix.rotate(90.0f, 1.0f, 0.0f, 0.0f);

				drawMeshObject(2, projection, modelMatrix, barrelObject,currentModel+1);
				//glUniform1i(textureUniformHandle, textureHandleArr[iterNumber] );	
				//glDrawElements(GL_TRIANGLES, modelVector[currentModel]->numVerArray[currentMaterial], GL_UNSIGNED_INT, 0);	
				
			}
		}  	 
	}
	else if(currentModel = 1)
	{	
		//Vehicle control
		float scale_x = cos(vehicleAngle* PI / 180.0); 
		float scale_y = sin(vehicleAngle* PI / 180.0);

		float vehicleVelocity_x = vehicleSpeed * scale_x;
		float vehicleVelocity_y = vehicleSpeed * scale_y;

		vehiclePos_x = vehiclePos_x + vehicleVelocity_x;
		vehiclePos_y = vehiclePos_y + vehicleVelocity_y;

		//vehicle boundaries
		if(vehiclePos_x > groundX/2 - 3){ vehiclePos_x = groundX/2 - 3; vehicleSpeed = 0;}
		if(vehiclePos_x < -groundX/2 + 3){ vehiclePos_x = -groundX/2 + 3; vehicleSpeed = 0;}
		if(vehiclePos_y > groundX/2 - 3){ vehiclePos_y = groundX/2 - 3; vehicleSpeed = 0;}
		if(vehiclePos_y < -groundX/2 + 3){ vehiclePos_y = -groundX/2 + 3; vehicleSpeed = 0;}
		
		//printf("vehicleAngle: %.2f , vehiclePos_y: %.2f , vehiclePos_x: %.2f  \n", vehicleAngle, vehiclePos_y, vehiclePos_x );

		modelMatrix.copy(cameraMatrix);
		modelMatrix.translate(vehiclePos_y, 0.5f ,vehiclePos_x);
		modelMatrix.scale(0.5f, 0.5f, 0.5f);
		modelMatrix.rotate(vehicleAngle,0.0f,1.0f,0.0f);

		drawMeshObject(2, projection, modelMatrix, vehicleObject,currentModel+1);
		
	}
}

void setLight(ShaderStruct shader)
{
	float lightPos[4] = { -1.0f, 0.0f, 0.0f, 0.0f };	// w-component 0 gives directional, 1 gives positional

	Matrix4f lightMatrix;

	lightMatrix.copy(cameraMatrix); 
	//lightMatrix.translate(0.0f, 10.0f, 0.0f);
			
	lightMatrix.transformVec4f(lightPos);	// Transform our array of 4 floats by this matrix
	glUniform4fv(shader.lightposUniformHandle, 1, lightPos);	// Set already transformed light position
}

void render() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	projection.loadPerspective(80, (double)WINDOW_WIDTH/(double)WINDOW_HEIGHT, 0.1, 500.0); // perspective projection
	
	/////////////////camera//////////////
	cameraMatrix.loadIdentity();

	cameraMatrix.translate(0.0f, 0.0f, -7.0f);
	cameraMatrix.rotate(180-vehicleAngle,0.0f,1.0f,0.0f);
	cameraMatrix.rotate(cameraRotDelta, 0.0f, 1.0f, 0.0f); //to be able to see the vehicle from the side..
	cameraMatrix.translate(-vehiclePos_y, -2.0f, -vehiclePos_x);
	
	///////////////Ground//////////////////
	//groundMatrix.copy(cameraMatrix);
	////
	//groundMatrix.translate(0.0f, 0.2f, 0.0f);    
 //   groundMatrix.scale(groundX, groundZ, groundZ); 
	//groundMatrix.rotate(90.0f,1.0f,0.0f,0.0f);
	//drawMeshObject(1,projection, groundMatrix, groundObject,0);	

	//////////////Terrain////////////////
	groundMatrix.copy(cameraMatrix);
	float scaleT = 2.0f;
	groundMatrix.translate(scaleT*-16.0f, 0.0f, scaleT*16.0f);    
    groundMatrix.scale(scaleT, scaleT, scaleT); 
	//groundMatrix.rotate(90.0f,1.0f,0.0f,0.0f);
	drawMeshObject(3,projection, groundMatrix, terrainObject,8);

	//////////Light//////////////////////
	setLight(programScenery);

	///////////vehicle and scenery////////////
	unsigned int drawIncr = 1;
	for(int b=0; b<numberOfModels;b++)
	{
		for(int a=0; a<modelVector[b]->materialCount;a++) 
		{
			drawModel(a,b,drawIncr);
			drawIncr += 1; //this only works like this if every material have a texture!
		}
	}

	/////////////skybox/////////////////
	modelMatrix.loadIdentity();
	// And apply this models own transformations
	modelMatrix.translate(0.0f, 0.0f, 0.0f);    
    modelMatrix.scale(100.0f, 100.0f, 100.0f);    
	modelMatrix.rotate(180-vehicleAngle,0.0f,1.0f,0.0f);
	modelMatrix.rotate(180,0.0f,0.0f,1.0f); //remove to get gray sky!!!

	drawMeshObject(1, projection, modelMatrix, skyboxObject,9);
	
	glBindVertexArray(0);

	glutSwapBuffers();
	glFlush();

}
void mySpecialKeyFunc( int key, int x, int y ) //using special key func to use the glut specific keys
{
	switch ( key ) {
		case GLUT_KEY_LEFT:
			vehicleAngle += VEHICLE_ROT_AMOUNT;
			break;
		case GLUT_KEY_RIGHT:
			vehicleAngle -= VEHICLE_ROT_AMOUNT;
			break;
		case GLUT_KEY_UP:
			vehicleSpeed += VEHICLE_ACCEL_AMOUNT;
			break;
		case GLUT_KEY_DOWN:
			vehicleSpeed -= VEHICLE_ACCEL_AMOUNT;
			break;
	}
	glutPostRedisplay();

}

void keyboardDown(unsigned char key, int x, int y) {

	//to be able to get a better look of the hemisphere lightning.
	switch (key) {
		case 'a':
			cameraRotDelta += CAMERA_ROT_AMOUNT;
			break;
		case 'd':
			cameraRotDelta -= CAMERA_ROT_AMOUNT;
			break;        
	}

	// Tell GLUT the window needs to be redrawn. Important, because unless something 'dirties'
	// the window, GLUT does call the render function
	glutPostRedisplay();
}

void initModelsAndShaders(){
	
	programSkyBox = loadShader("sky.vert", "sky.frag");
	programVehicle = loadShader("vehicle.vert", "vehicle.frag");
	programScenery = loadShader("scenery.vert", "scenery.frag");


	//////////Ground///////////////
	ground = new Scenery;
	ground->construct();
	groundVaoHandle = createVAO(programScenery,ground->vertices, ground->normals, ground->tex_coord ,ground->coloursA, ground->coloursD, ground->coloursS, ground->indices,
	ground->vertCount, ground->normCount, ground->texCount, ground->coloCount, ground->indCount);

	groundObject = MeshObject(
		programScenery,
		groundVaoHandle, 
		ground->indCount 
	);	
	///////////barrels//////////////
	loadModels(); 

	barrelObject = MeshObject(
		programScenery,
		modelVaoHandle[1],
		modelVector[0]->numVerArray[0] 
	);	

	////////////////Terrain///////////////////
	setupHeightMap();
	terrainVaoHandle = createVAO(programScenery,vertexArray,normalArray,texArray,colorArray,colorArray,colorArray,indicesTer,(MAP_Z-1)*(MAP_X-1)*12,(MAP_Z-1)*(MAP_X-1)*12*VALS_PER_NORMAL,(MAP_Z-1)*(MAP_X-1)*12*VALS_PER_TEX,(MAP_Z-1)*(MAP_X-1)*12*VALS_PER_COLOUR,(MAP_Z-1)*(MAP_X-1) *12/3);

	terrainObject = MeshObject(
		programScenery,
		terrainVaoHandle,
		(MAP_Z-1)*(MAP_X-1) *12/3
	);

	setUpLight(programScenery);

	////////Skybox//////
	skycube = new Cube(1.0f,1.0f,1.0f,1.0f, 1.0f,1.0f,1.0f,1.0f,  0.1f,0.1f,0.1f,1.0f);
	skyVaoHandle = createVAO(programSkyBox, skycube->vertices,skycube->normals,skycube->tex_coord,skycube->coloursA,skycube->coloursD,skycube->coloursS,skycube->indices,skycube->vertCount,skycube->normCount,skycube->texCount,skycube->coloCount,skycube->indCount);	 
	
	skyboxObject = MeshObject(
		programSkyBox,
		skyVaoHandle,
		skycube->indCount 
	);	
	/////////Vehicle//////

	vehicleObject = MeshObject(
		programVehicle,
		modelVaoHandle[2],
		modelVector[1]->numVerArray[0] 
	);	

}

void timer(int i) {
    glutTimerFunc(TICK_MILLS, timer, 0);
	glutPostRedisplay();
}

int main(int argc, char **argv) {

	std::cout << "Controls: AD- Rotate left and right WX- Rotate up and down FB- Translate forward and backward\n";

	// Set up GLUT window
	glutInit(&argc, argv);
	glutInitWindowPosition(520, 220);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("My openGL world");

	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glFrontFace(GL_CCW);

    InitializeTerrain();
	initTexture();
	initModelsAndShaders();
	glutTimerFunc(TICK_MILLS, timer, 0);
	// Here we set a new function callback which is the GLUT handling of keyboard input
	glutKeyboardFunc(keyboardDown);
	glutSpecialFunc(mySpecialKeyFunc);
	glutDisplayFunc(render);
	glutMainLoop();

	return 0;
}
