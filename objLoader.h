
#ifndef OBJ_LOADER_H
#define OBJ_LOADER_H

// Object loading and parsing library.
// Original version at http://www.kixor.net/dev/objloader/
// Modified by Anthony Dick

#include "obj_parser.h"

class objLoader
{
public:
	objLoader() {}
	~objLoader()
	{
		delete_obj_data(&data);
	}

    // Use this function to load an OBJ model from file
	int load(const char *filename);

    // Arrays containing vertex, normal and texture coordinate data
    // See objTester.cpp for an example of how to get to the data.
	obj_vector **vertexList;
	obj_vector **normalList;
	obj_vector **textureList;

    // Arrays of face data and material data
    // See obj_parser.h for definitions of obj_face and obj_material types
	obj_face **faceList;
	obj_material **materialList;

    // Number of items in each of the above arrays
	int vertexCount;
	int normalCount;
	int textureCount;

	int faceCount;
	int materialCount;

private:
	obj_scene_data data;
};

#endif
