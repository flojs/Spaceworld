#include "scenery.h"
#include <algorithm>

void Scenery::construct(){

	vertices = new float[POLYGON_NUM_VERTICES*VALS_PER_VERT];
	normals = new float[POLYGON_NUM_VERTICES*VALS_PER_NORMAL];
	coloursA = new float[POLYGON_NUM_VERTICES*VALS_PER_COLOUR];   
	coloursD = new float[POLYGON_NUM_VERTICES*VALS_PER_COLOUR]; 
	coloursS = new float[POLYGON_NUM_VERTICES*VALS_PER_COLOUR]; 
	indices = new unsigned int[POLYGON_NUM_SQUARES*VALS_PER_INDEX];
	tex_coord = new float[POLYGON_NUM_SQUARES*VALS_PER_INDEX*VALS_PER_TEX];

	/////////////////////

	float vertArr[] = {
		 -1.0f, -1.0f, 0.0f , 1.0f, -1.0f, 0.0f ,1.0f,  1.0f, 0.0f ,-1.0f,  1.0f, 0.0f 
	};

	std::copy(vertArr, vertArr + (POLYGON_NUM_VERTICES*VALS_PER_VERT), vertices);

	float normArr [] = {
		0.0f, 0.0f, -1.0f,  0.0f, 0.0f, -1.0f,  0.0f, 0.0f, -1.0f,  0.0f, 0.0f, -1.0f, // front
	};

	std::copy(normArr, normArr + (POLYGON_NUM_VERTICES*VALS_PER_NORMAL), normals);

	float aColArr[] = {
			1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f
	};

	std::copy(aColArr, aColArr + (POLYGON_NUM_VERTICES*VALS_PER_COLOUR), coloursA);
	
	float dColArr[] = {
			1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f
	};
	
	std::copy(dColArr, dColArr + (POLYGON_NUM_VERTICES*VALS_PER_COLOUR), coloursD);
	
	float sColArr[] = {
			1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f
	};
	
	std::copy(sColArr, sColArr + (POLYGON_NUM_VERTICES*VALS_PER_COLOUR), coloursS);
           
    unsigned int indiArr[] = {
        0,1,2,3
    };
	std::copy(indiArr, indiArr + (POLYGON_NUM_VERTICES* VALS_PER_INDEX), indices);
    
    float texArr[] = {
        0.0f, 0.0f,  2.0f, 0.0f, 2.0f, 2.0f, 0.0f, 2.0f
    };

	std::copy(texArr, texArr + (POLYGON_NUM_SQUARES*VALS_PER_INDEX*VALS_PER_TEX), tex_coord);


		// Counts of array elements
	vertCount = POLYGON_NUM_VERTICES * VALS_PER_VERT;
	normCount = POLYGON_NUM_VERTICES * VALS_PER_NORMAL;
	coloCount = POLYGON_NUM_VERTICES * VALS_PER_COLOUR;   
	indCount = POLYGON_NUM_SQUARES*VALS_PER_INDEX;
	texCount = POLYGON_NUM_SQUARES*VALS_PER_INDEX*VALS_PER_TEX;

}

void Scenery::render()
{

}

