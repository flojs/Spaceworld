#ifndef CUBE_H
#define CUBE_H

#define VALS_PER_VERT 3
#define VALS_PER_NORMAL 3
#define VALS_PER_COLOUR 4
#define VALS_PER_TEX 2 

#define VALS_PER_INDEX 4	// The number of vertices in our square
#define CUBE_NUM_SQUARES 6      // number of squares in a cube
#define CUBE_NUM_VERTICES 24     // number of vertices in a cube`
#define CUBE_NUM_NORMALS 24

class Cube {
public:

	Cube (float RA = 1.0f,float GA = 1.0f,float BA = 1.0f,float AA = 1.0f, float RD = 1.0f,float GD = 1.0f,float BD = 1.0f,float AD = 1.0f, float RS = 1.0f,float GS = 1.0f,float BS = 1.0f,float AS = 1.0f );

	float *vertices;		// Vertex position (x,y,z)
	float *normals;			// Normal vector (x,y,z)
	float *tex_coord;
    float *coloursA;         // Colours Ambient
	float *coloursD;         // Colours Diffuse
	float *coloursS;         // Colours Specular
	unsigned int *indices;	// Element indices

		// Counts of array elements
	int vertCount;
	int normCount;
	int coloCount;
	int texCount;
	int indCount;

	~Cube();
};

#endif
