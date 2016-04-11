#pragma once
#include "GLew/glew.h"
#include "Mesh.h"
#include "Octant.h"

#include <vector>

#ifndef DYNAMICMESH_H
#define DYNAMICMESH_H

struct dBufferData {
	GLfloat x;
	GLfloat y;
	GLfloat z;
	GLfloat nx;
	GLfloat ny;
	GLfloat nz;
};

#endif

//! A class representing a modifiable 3D mesh 
class DynamicMesh : public Mesh{
	friend class Octant;

  public:
	DynamicMesh();
	~DynamicMesh();

	void render();

	void createBuffers();

	void genTableTex();

	GLuint tmpFbo;

	GLuint edgeTex;
	GLuint voxelTex;
	const int voxelRes = 256;

	GLuint edgeTableTex;
	GLuint triTableTex;

	GLuint singlePointvao;          // Vertex array object, the main handle for geometry
	
	GLuint nonEmptyCellsBuffer;
	GLuint nonEmptyCellsArray;
	
	GLuint nonEmptyEdgesBuffer;
  private:

	  //TODO: remove one zero
	const int MAX_NR_OF_VERTICES = 10000000;
	//! negative index of the latest removed vertex
	/*! vInfoArray's edgepointer contains the negative index of an empty slot in the vertexArray*/
	std::vector<int> emptyVStack;

	const int MAX_NR_OF_TRIANGLES = 2 * MAX_NR_OF_VERTICES;
	//! negative index of the latest removed triangle
	/*! triEPtr contains the negative index of an empty slot in the triangleArray*/
	std::vector<int> emptyTStack;

	//largest index in the vertexArray where values exist 
	int vertexCap;
	//largest index in the indexArray where values exist 
	int triangleCap;

	GLuint singlePointvertexbuffer; // Buffer ID to bind to GL_ARRAY_BUFFER

};