#include "Octant.h"
#include <vector>
#include "DynamicMesh.h"

#include <iostream>

Octant::Octant() {
	parent = nullptr;
	depth = NULL;



}

Octant::Octant(int _depth, Octant* _parent, float x, float y, float z, float _halfDim, bool _fill) {
	parent = _parent;
	depth = _depth;
	pos[0] = { x }; pos[1] = { y }; pos[2] = { z };
	halfDim = _halfDim;

	voxelData = { nullptr };
}


Octant::~Octant() {

	/*if (depth == MAX_DEPTH) {
		int scalarNR = std::pow(2, (10 - depth));

		for (int i = 0; i < scalarNR; i++)
			for (int j = 0; j < scalarNR; j++)
				delete scalarValue[i][j];

		for (int i = 0; i < scalarNR; i++)
			delete scalarValue[i];

		delete scalarValue;
	}
	else {
		if (child[0] != nullptr) {
			delete child[0];
			delete child[1];
			delete child[2];
			delete child[3];
			delete child[4];
			delete child[5];
			delete child[6];
			delete child[7];
		}

		delete scalarValue[0][0];
		delete scalarValue[0];
		delete scalarValue;
	}*/

	if (child[0] != nullptr) {
		delete child[0];
		child[0] = nullptr;
		delete child[1];
		delete child[2];
		delete child[3];
		delete child[4];
		delete child[5];
		delete child[6];
		delete child[7];
	}
}

//do NOT use on MAX_DEPTH octants
void Octant::partition() {
	//scalarValue = 0;
	//isoBool = false;

	float d = halfDim / 2.0f;
	child[0] = new Octant(depth + 1, this, pos[0] - d, pos[1] - d, pos[2] - d, d, isoBool);
	child[1] = new Octant(depth + 1, this, pos[0] - d, pos[1] - d, pos[2] + d, d, isoBool);
	child[2] = new Octant(depth + 1, this, pos[0] - d, pos[1] + d, pos[2] - d, d, isoBool);
	child[3] = new Octant(depth + 1, this, pos[0] - d, pos[1] + d, pos[2] + d, d, isoBool);
	child[4] = new Octant(depth + 1, this, pos[0] + d, pos[1] - d, pos[2] - d, d, isoBool);
	child[5] = new Octant(depth + 1, this, pos[0] + d, pos[1] - d, pos[2] + d, d, isoBool);
	child[6] = new Octant(depth + 1, this, pos[0] + d, pos[1] + d, pos[2] - d, d, isoBool);
	child[7] = new Octant(depth + 1, this, pos[0] + d, pos[1] + d, pos[2] + d, d, isoBool);


}

void Octant::collisionCheck() {


}

//do NOT use on MAX_DEPTH octants
void Octant::deAllocate(DynamicMesh* _mesh) {
	scalarValue = child[0]->scalarValue;
	isoBool = child[0]->isoBool;
	for (int i = 0; i < 8; i++) {
		// child has no children 
		if (child[i]->child[0] == nullptr) {
			// scalarValue is allocated
			if (child[i]->triangles != nullptr) {
				//delete triangle scalarValue
				for (int j = 0; j < child[i]->tCount; j++){
					_mesh->emptyTStack.push_back(child[i]->triangles[j]);
				}
				delete[] child[i]->triangles;
				child[i]->tCount = 0;
				child[i]->triangles = nullptr;
				
				//delete vertex scalarValue
				if (child[i]->vertices[0] != -1){
					_mesh->emptyVStack.push_back(child[i]->vertices[0]);
					child[i]->vertices[0] = -1;
				}
				if (child[i]->vertices[1] != -1){
					_mesh->emptyVStack.push_back(child[i]->vertices[1]);
					child[i]->vertices[1] = -1;
				}
				if (child[i]->vertices[2] != -1){
					_mesh->emptyVStack.push_back(child[i]->vertices[2]);
					child[i]->vertices[2] = -1;
				}
			}
		}
		// child has children
		else {
			child[i]->deAllocate(_mesh);
		}
		// clean up
		delete child[i];	
	}
	child[0] = nullptr;
}

//do NOT use on MAX_DEPTH or (MAX_DEPTH - 1)  octants
void Octant::checkHomogeneity() {
	//int count = 0;
	for (int i = 0; i < 8; i++){
		if (child[i]->child[0] != nullptr)
			return;
	}
	scalarValue = child[0]->scalarValue;
	isoBool = child[0]->isoBool;
	for (int i = 0; i < 8; i++){
		delete child[i];
	}

	child[0] = nullptr;
	
	if (parent != nullptr)
		parent->checkHomogeneity();
}

//do NOT use on MAX_DEPTH or (MAX_DEPTH - 1)  octants
void Octant::checkHomogeneity(std::vector<octantStackElement>& octStack) {
	//int count = 0;
	for (int i = 0; i < 8; i++){
		if (child[i]->child[0] != nullptr)
			return;
	}
	scalarValue = child[0]->scalarValue;
	isoBool = child[0]->isoBool;
	for (int i = 0; i < 8; i++){
		delete child[i];
	}

	octStack.pop_back();

	child[0] = nullptr;

	if (parent != nullptr)
		parent->checkHomogeneity(octStack);
}

void Octant::findNeighbors(Octant* _oNeighbors[7]) {
	float hDim = halfDim;
	float fDim = hDim * 2;
	float addDim = hDim - 0.001953125f;//minDim;
	float cornerPos[3];
	float nPos[7][3];
	Octant* tmpOct;
	cornerPos[0] = pos[0] + addDim;
	cornerPos[1] = pos[1] + addDim;
	cornerPos[2] = pos[2] + addDim;
	//find neighbours -------------------------------------
	//childOct->pos
	nPos[0][0] = cornerPos[0] - fDim;	nPos[0][1] = cornerPos[1] - fDim;	nPos[0][2] = cornerPos[2] - fDim;
	nPos[1][0] = cornerPos[0];			nPos[1][1] = cornerPos[1] - fDim;	nPos[1][2] = cornerPos[2] - fDim;
	nPos[2][0] = cornerPos[0];			nPos[2][1] = cornerPos[1] - fDim;	nPos[2][2] = cornerPos[2];
	nPos[3][0] = cornerPos[0] - fDim;	nPos[3][1] = cornerPos[1] - fDim;	nPos[3][2] = cornerPos[2];
	nPos[4][0] = cornerPos[0] - fDim;	nPos[4][1] = cornerPos[1];			nPos[4][2] = cornerPos[2] - fDim;
	nPos[5][0] = cornerPos[0];			nPos[5][1] = cornerPos[1];			nPos[5][2] = cornerPos[2] - fDim;
	nPos[6][0] = cornerPos[0] - fDim;	nPos[6][1] = cornerPos[1];			nPos[6][2] = cornerPos[2];

	for (int j = 0; j < 7; j++){
		//TODO: replace temp values of thjs jf
		if ((nPos[j][0] > 0.5f || nPos[j][0] < -0.5f ||
			nPos[j][1] > 0.5f || nPos[j][1] < -0.5f ||
			nPos[j][2] > 0.5f || nPos[j][2] < -0.5f)){
			std::cout << "utanfor";
		}

		tmpOct = parent;
		while (nPos[j][0] > tmpOct->pos[0] + tmpOct->halfDim || nPos[j][0] < tmpOct->pos[0] - tmpOct->halfDim ||
			nPos[j][1] > tmpOct->pos[1] + tmpOct->halfDim || nPos[j][1] < tmpOct->pos[1] - tmpOct->halfDim ||
			nPos[j][2] > tmpOct->pos[2] + tmpOct->halfDim || nPos[j][2] < tmpOct->pos[2] - tmpOct->halfDim){

			tmpOct = tmpOct->parent;
		}
		while (tmpOct->child[0] != nullptr){
			for (int k = 0; k < 8; k++){
				if (nPos[j][0] < tmpOct->child[k]->pos[0] + tmpOct->child[k]->halfDim && nPos[j][0] > tmpOct->child[k]->pos[0] - tmpOct->child[k]->halfDim &&
					nPos[j][1] < tmpOct->child[k]->pos[1] + tmpOct->child[k]->halfDim && nPos[j][1] > tmpOct->child[k]->pos[1] - tmpOct->child[k]->halfDim &&
					nPos[j][2] < tmpOct->child[k]->pos[2] + tmpOct->child[k]->halfDim && nPos[j][2] > tmpOct->child[k]->pos[2] - tmpOct->child[k]->halfDim){
					tmpOct = tmpOct->child[k];
					break;
				}
			}
		}
		_oNeighbors[j] = tmpOct;
	}
}

void Octant::render(MatrixStack* MVstack, GLint locationMV) {
	
	if (child[0] != nullptr){
		child[0]->render(MVstack, locationMV);
		child[1]->render(MVstack, locationMV);
		child[2]->render(MVstack, locationMV);
		child[3]->render(MVstack, locationMV);
		child[4]->render(MVstack, locationMV);
		child[5]->render(MVstack, locationMV);
		child[6]->render(MVstack, locationMV);
		child[7]->render(MVstack, locationMV);
	}
	else {
		MVstack->push();
			MVstack->translate(&pos);
			glUniformMatrix4fv(locationMV, 1, GL_FALSE, MVstack->getCurrentMatrix());
			//shape->render();
		MVstack->pop();
	}
}