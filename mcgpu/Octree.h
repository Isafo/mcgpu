#pragma once
#include "Octant.h"
class Octree
{
public:
	Octree(float _dim);

	~Octree();

	Octant* root;

	//relates to MAX_DEPTH in class Octant. 
	//Must always be equal to the vector length of the vector that spans the corners in a voxel
	const double voxDiagonalLength = 0.006928203230;//0.00346410161514;
	
	float halfDim;
	//int scalarRes[3];

	void render(MatrixStack* MVstack, GLint locationMV);

private:

	LineCube* shape;
	
};

