#include "ModelViewer.h"
#include "objLoader.h"
#include <stdio.h>
#include <math.h>  
#include <algorithm>

void ModelViewer::load(const char *filename)
{	
	objData = new objLoader(); 
	objData->load(filename);
	vertexCount = objData->vertexCount;
	vertexArrayCount = objData->faceCount * 3 * 3; //faces*vertices*xyz
	materialCount = objData->materialCount;
	textureCount = objData->textureCount;

	printf("Number of vertices: %i\n", objData->vertexCount);
	printf("Number of vertex normals: %i\n", objData->normalCount);
	printf("Number of texture coordinates: %i\n", objData->textureCount);
	printf("Number of materials: %i\n", materialCount);
}

void ModelViewer::render(int currentMaterial)
{	
	vertexArray = new float[vertexArrayCount];
	normalArray = new float[vertexArrayCount];
	textureArray = new float[vertexArrayCount*2];
	
	numVerArray = new int[materialCount];

	unsigned int teller = 0;
	unsigned int incr = 0;

	/////Materials////
	
	mtl = objData->materialList[currentMaterial];
	texture_filename = mtl->texture_filename;
	printf("Texture name:%s\n", texture_filename);
	

/*	printf(" name: %s", mtl->name);
	printf(" amb: %.2f ", mtl->amb[0]);
	printf("%.2f ", mtl->amb[1]);
	printf("%.2f\n", mtl->amb[2]);
	
	printf(" diff: %.2f ", mtl->diff[0]);
	printf("%.2f ", mtl->diff[1]);
	printf("%.2f\n", mtl->diff[2]);

	printf(" spec: %.2f ", mtl->spec[0]);
	printf("%.2f ", mtl->spec[1]);
	printf("%.2f\n", mtl->spec[2]);		
	printf(" shiny: %.2f\n", mtl->shiny);

	printf(" texture: %s\n", mtl->texture_filename);
	printf("\n");
*/
	////////////faces////////////
	
	for(int i=0; i<objData->faceCount; i++)
	{
		obj_face *o = objData->faceList[i];
		
		if(o->material_index == currentMaterial) //if face has material equal to currentMaterial:
		{
			for(int j=0; j<3; j++) //for each of the 3 vertices
			{	
				//printf("currentMaterial, i, j, teller: %i , %i , %i , %i\n",currentMaterial,i,j, teller+1);
				//printf("vec: %.2f , %.2f , %.2f \n",objData->vertexList[ o->vertex_index[j]]->e[0] ,objData->vertexList[ o->vertex_index[j] ]->e[1],objData->vertexList[ o->vertex_index[j]]->e[2] );
				teller++;
				vertexArray[i*9+j*3  ] = objData->vertexList[ o->vertex_index[j] ]->e[0];
				vertexArray[i*9+j*3+1] = objData->vertexList[ o->vertex_index[j] ]->e[1];
				vertexArray[i*9+j*3+2] = objData->vertexList[ o->vertex_index[j] ]->e[2];

				normalArray[i*9+j*3  ] = objData->normalList[ o->normal_index[j] ]->e[0];
				normalArray[i*9+j*3+1] = objData->normalList[ o->normal_index[j] ]->e[1];
				normalArray[i*9+j*3+2] = objData->normalList[ o->normal_index[j] ]->e[2];
				//printf("i*9+j*3: %i %i %i\n",i*9+j*3,i*9+j*3+1,i*9+j*3+2);

				// old if(objData->textureCount > 0) //worked somehow..
				
				if(mtl->texture_filename[0] != '\0' )
				{ 
					//printf("texture \n");
					textureArray[incr] = objData->textureList[ o->texture_index[j] ]->e[0];
					textureArray[incr+1] = objData->textureList[ o->texture_index[j] ]->e[1];
					
					incr += 2;

					//printf("Incr,Incr+1: %i %i\n",incr,incr+1);
					//printf("TextureCoord: %.2f , %.2f  \n",objData->textureList[ o->texture_index[j] ]->e[0] ,objData->textureList[ o->texture_index[j] ]->e[1]);
				}
				else
				{
					printf("no texture \n");
					textureArray[incr] = 0.0f; // = {}; ???
				}
			}
		}
	}
	numVerArray[currentMaterial] = teller; 
	printf("incr, teller: %i %i \n",incr, teller);
	printf("numVerArray[currentMaterial], m: %i , %i \n",numVerArray[currentMaterial],currentMaterial);
	
}

float ModelViewer::maxXYZ()
{
	float maxABSValue=fabs(vertexArray[0]);
	
    for(int a=0;a<vertexArrayCount;a++)
	{
		if(fabs(vertexArray[a])>maxABSValue && fabs(vertexArray[a]) < 100000.00) //quick fix to avoid the uninitialised values
		{
			maxABSValue=vertexArray[a];
		}
    }
	return maxABSValue;
}
