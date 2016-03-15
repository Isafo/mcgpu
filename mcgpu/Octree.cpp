#include "Octree.h"


Octree::Octree(float _dim)
{
	halfDim = _dim/2.0f;
	root = new Octant(0, nullptr, 0.0f, 0.0f, 0.0f, halfDim, false);

	root->partition();

	shape = new LineCube(0.0f, 0.0f, 0.0f, halfDim * 2, halfDim * 2, halfDim * 2);

}


Octree::~Octree()
{

}

void Octree::render(MatrixStack* MVstack, GLint locationMV) {

		MVstack->push();
		MVstack->translate(root->pos);
		glUniformMatrix4fv(locationMV, 1, GL_FALSE, MVstack->getCurrentMatrix());
		shape->render();
		MVstack->pop();

}