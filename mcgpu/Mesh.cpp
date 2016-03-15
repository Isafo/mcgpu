#include "Mesh.h"


Mesh::Mesh()
{
}


Mesh::~Mesh()
{
}

void Mesh::getOrientation(float* mat){
	for (int i = 0; i < 16; i++) {
		mat[i] = orientation[i];
	}
}