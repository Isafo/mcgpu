#pragma once
#include "Mesh.h"
#include "MatrixStack.h"

#include <glm/vec3.hpp> // glm::vec3

#include <math.h>
#include <vector>

class DynamicMesh;

class Octant;

struct octantStackElement {
	Octant* octant;
	int index{ 0 };
};

class Octant
{
public:
	Octant();
	Octant(int _depth, Octant* _parent, float x, float y, float z, float _hafDim, bool _fill);
	~Octant();

	void deAllocate(DynamicMesh* _mesh);

	static const int MAX_DEPTH = 8;

	void allocateData();
	void deAllocateData();

	void partition();

	void collisionCheck();

	void checkHomogeneity();
	void checkHomogeneity(std::vector<octantStackElement>& octStack);

	void findNeighbors(Octant* _oNeighbors[7]);

	//LineCube* shape;
	void render(MatrixStack* MVstack, GLint locationMV);

	glm::vec3 pos;
	float halfDim;

	Octant* child[8];
	Octant* parent;
	int depth;

	unsigned int vBufferIndex;
	unsigned int tBufferIndex;
	unsigned char*** voxelData;

private:
	


};



