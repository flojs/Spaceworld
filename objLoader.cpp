
#include "objLoader.h"
#include "obj_parser.h"
#include <stdio.h>

int objLoader::load(const char *filename)
{
	int no_error = 1;
	no_error = parse_obj_scene(&data, filename);
	if (no_error)
	{
		this->vertexCount = data.vertex_count;
		this->normalCount = data.vertex_normal_count;
		this->textureCount = data.vertex_texture_count;
        
		this->faceCount = data.face_count;
		this->materialCount = data.material_count;

		this->vertexList = data.vertex_list;
		this->normalList = data.vertex_normal_list;
		this->textureList = data.vertex_texture_list;

		this->faceList = data.face_list;
		this->materialList = data.material_list;
	}

	return no_error;
}
