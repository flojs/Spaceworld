#ifndef MODEL_VIEWER_H
#define MODEL_VIEWER_H

#include "objLoader.h"
#include <stdio.h>


class ModelViewer
{
public:
	ModelViewer() {}

	void load(const char *filename);
	void render(int currentMaterial);
	float maxXYZ();

	float *vertexArray;
	float *normalArray;
	float *textureArray;
	int *numVerArray;

	objLoader *objData;
	
	obj_material *mtl;

	int vertexArrayCount; 
	int vertexCount;

	int materialCount;
	int textureCount;
	char *texture_filename;
	
	
};


#endif