#include "cube.h"
#include <algorithm>

Cube::Cube(float RA , float GA , float BA, float AA, float RD , float GD , float BD, float AD, float RS , float GS , float BS, float AS){

	vertices = new float[CUBE_NUM_VERTICES * VALS_PER_VERT];
	normals = new float[CUBE_NUM_NORMALS * VALS_PER_NORMAL];
	coloursA = new float[CUBE_NUM_VERTICES * VALS_PER_COLOUR];   
	coloursD = new float[CUBE_NUM_VERTICES * VALS_PER_COLOUR]; 
	coloursS = new float[CUBE_NUM_VERTICES * VALS_PER_COLOUR]; 
	indices = new unsigned int[CUBE_NUM_SQUARES * VALS_PER_INDEX];
	tex_coord = new float[CUBE_NUM_SQUARES*VALS_PER_INDEX*VALS_PER_TEX];

	
	float vertArr [] =  {  //rem to change normals with face order!
			  -1.0f, -1.0f,  1.0f ,   1.0f, -1.0f,  1.0f,   1.0f,  1.0f,  1.0f ,  -1.0f,  1.0f,  1.0f, //front
			1.0f, -1.0f,  1.0f ,   1.0f, -1.0f, -1.0f,   1.0f,  1.0f, -1.0f ,   1.0f,  1.0f,  1.0f, //right
			1.0f, -1.0f, -1.0f ,  -1.0f, -1.0f, -1.0f,   -1.0f,  1.0f, -1.0f ,  1.0f,  1.0f, -1.0f, // back
			-1.0f, -1.0f, -1.0f ,  -1.0f, -1.0f,  1.0f,  1.0f,  -1.0f,  1.0f ,  1.0f,  -1.0f, -1.0f, //bottom
			-1.0f,  1.0f,  1.0f ,   1.0f,  1.0f,  1.0f,   1.0f,  1.0f, -1.0f ,  -1.0f,  1.0f, -1.0f, //top
			-1.0f, -1.0f, -1.0f ,   -1.0f, -1.0f, 1.0f,   -1.0f, 1.0f,  1.0f ,  -1.0f,  1.0f, -1.0f  //left
			
	};
	std::copy(vertArr, vertArr + (CUBE_NUM_VERTICES * VALS_PER_VERT), vertices);
	
	float normArr [] = {
		 

			0.0f, 0.0f, 1.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f, 1.0f, // front
			1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f, // right
			0.0f, 0.0f,-1.0f,  0.0f, 0.0f,-1.0f,  0.0f, 0.0f,-1.0f,  0.0f, 0.0f,-1.0f, // back
			0.0f,-1.0f, 0.0f,  0.0f,-1.0f, 0.0f,  0.0f,-1.0f, 0.0f,  0.0f,-1.0f, 0.0f, // bottom
			0.0f, 1.0f, 0.0f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f, 0.0f, // top
			-1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f  // left
	};

	std::copy(normArr, normArr + (CUBE_NUM_NORMALS * VALS_PER_NORMAL), normals);
	
	float colorArrA [] =  {
			RA,GA,BA,AA, RA,GA,BA,AA, RA,GA,BA,AA, RA,GA,BA,AA,
			RA,GA,BA,AA, RA,GA,BA,AA, RA,GA,BA,AA, RA,GA,BA,AA,
			RA,GA,BA,AA, RA,GA,BA,AA, RA,GA,BA,AA, RA,GA,BA,AA,
			RA,GA,BA,AA, RA,GA,BA,AA, RA,GA,BA,AA, RA,GA,BA,AA,
			RA,GA,BA,AA, RA,GA,BA,AA, RA,GA,BA,AA, RA,GA,BA,AA,
			RA,GA,BA,AA, RA,GA,BA,AA, RA,GA,BA,AA, RA,GA,BA,AA
	};    

	std::copy(colorArrA, colorArrA + (CUBE_NUM_VERTICES * VALS_PER_COLOUR), coloursA);

	float colorArrD [] =  {
			RD,GD,BD,AD, RD,GD,BD,AD, RD,GD,BD,AD, RD,GD,BD,AD,
			RD,GD,BD,AD, RD,GD,BD,AD, RD,GD,BD,AD, RD,GD,BD,AD,
			RD,GD,BD,AD, RD,GD,BD,AD, RD,GD,BD,AD, RD,GD,BD,AD,
			RD,GD,BD,AD, RD,GD,BD,AD, RD,GD,BD,AD, RD,GD,BD,AD,
			RD,GD,BD,AD, RD,GD,BD,AD, RD,GD,BD,AD, RD,GD,BD,AD,
			RD,GD,BD,AD, RD,GD,BD,AD, RD,GD,BD,AD, RD,GD,BD,AD
	};    

	std::copy(colorArrD, colorArrD + (CUBE_NUM_VERTICES * VALS_PER_COLOUR), coloursD);
	
	float colorArrS [] =  {
			RS,GS,BS,AS, RS,GS,BS,AS, RS,GS,BS,AS, RS,GS,BS,AS,
			RS,GS,BS,AS, RS,GS,BS,AS, RS,GS,BS,AS, RS,GS,BS,AS,
			RS,GS,BS,AS, RS,GS,BS,AS, RS,GS,BS,AS, RS,GS,BS,AS,
			RS,GS,BS,AS, RS,GS,BS,AS, RS,GS,BS,AS, RS,GS,BS,AS,
			RS,GS,BS,AS, RS,GS,BS,AS, RS,GS,BS,AS, RS,GS,BS,AS,
			RS,GS,BS,AS, RS,GS,BS,AS, RS,GS,BS,AS, RS,GS,BS,AS
	};    

	std::copy(colorArrS, colorArrS + (CUBE_NUM_VERTICES * VALS_PER_COLOUR), coloursS);

	unsigned int indiArr [] = {
        0,1,2,3,
        4,5,6,7,
        8,9,10,11,
        12,13,14,15,
        16,17,18,19,
        20,21,22,23
    };

	std::copy(indiArr, indiArr + (CUBE_NUM_SQUARES * VALS_PER_INDEX), indices);

	    // texture coordinates
    float texArr[] = {
        0.0f, 0.0f,  1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 0.0f,  1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 0.0f,  1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 0.0f,  1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 0.0f,  1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 0.0f,  1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f
    };
	std::copy(texArr, texArr + (CUBE_NUM_SQUARES*VALS_PER_INDEX*VALS_PER_TEX), tex_coord);

		// Counts of array elements
	vertCount = CUBE_NUM_VERTICES * VALS_PER_VERT;
	normCount = CUBE_NUM_NORMALS * VALS_PER_NORMAL;
	coloCount = CUBE_NUM_VERTICES * VALS_PER_COLOUR;   
	indCount = CUBE_NUM_SQUARES * VALS_PER_INDEX;
	texCount = CUBE_NUM_SQUARES * VALS_PER_INDEX * VALS_PER_TEX;

}

Cube::~Cube(){
	delete vertices;		
	delete normals;			
    delete coloursA;         
	delete coloursD;        
	delete  coloursS;        
	delete  indices;	

}

