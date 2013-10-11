#ifndef SCENERY_H
#define SCENERY_H

#define VALS_PER_VERT 3
#define VALS_PER_NORMAL 3
#define VALS_PER_COLOUR 4
#define VALS_PER_INDEX 4		
#define VALS_PER_TEX 2          // number of dimensions of the texture data (if image, set it to 2)
#define POLYGON_NUM_SQUARES 1     
#define POLYGON_NUM_VERTICES 4 



class Scenery {
public:

	Scenery (){};
	void construct();
	void render();


	float *vertices;		// Vertex position (x,y,z)
	float *normals;			// Normal vector (x,y,z)
    float *coloursA;         // Colours Ambient
	float *coloursD;         // Colours Diffuse
	float *coloursS;         // Colours Specular
	unsigned int *indices;	// Element indices
	float *tex_coord;		//Texture coordinates 

		// Counts of array elements
	int vertCount;
	int normCount;
	int coloCount;    
	int indCount;
	int texCount;

	
};

#endif
