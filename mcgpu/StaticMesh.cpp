#include "StaticMesh.h"
#include <sstream>
#include <fstream>
#include <string>
#include <iostream>

StaticMesh::StaticMesh() {
}


StaticMesh::~StaticMesh() {
	//delete[] vertexArray;
	//delete[] triangleArray;
	//clean();
}

void StaticMesh::clean() {
	if (glIsVertexArray(vao)) {
		glDeleteVertexArrays(1, &vao);
	}
	vao = 0;

	if (glIsBuffer(vertexbuffer)) {
		glDeleteBuffers(1, &vertexbuffer);
	}
	vertexbuffer = 0;

	if (glIsBuffer(indexbuffer)) {
		glDeleteBuffers(1, &indexbuffer);
	}
	indexbuffer = 0;
}

void StaticMesh::render() {
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, (nrofTris) * sizeof(triangle), GL_UNSIGNED_INT, (void*)0);
	// (mode, vertex uN, type, element array buffer offset)
	glBindVertexArray(0);
}

void StaticMesh::createBuffers() {
	//triangle* indexP;
	//sBufferData * vertexP;
	//texCoords tempTex;
	//tempTex.u = 0.0f;
	//tempTex.v = 0.0f;

	////vertexP = &vertexArray[0];
	////indexP = &triangleArray[0];

	//// Generate one vertex array object (VAO) and bind it
	//glGenVertexArrays(1, &vao);
	//glBindVertexArray(vao);

	//// Generate two buffer IDs
	//glGenBuffers(1, &vertexbuffer);
	//glGenBuffers(1, &indexbuffer);

	//// Activate the vertex buffer
	//glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	//// Present our vertex coordinates to OpenGL
	//glBufferData(GL_ARRAY_BUFFER,
	//	(nrofVerts)*sizeof(sBufferData), NULL, GL_STREAM_DRAW);

	//vertexP = (sBufferData*)glMapBufferRange(GL_ARRAY_BUFFER, 0, sizeof(sBufferData) * nrofVerts,
	//	GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT | GL_MAP_UNSYNCHRONIZED_BIT);

	//for (int i = 0; i < nrofVerts; i++) {
	//	vertexP[i].x = vertexArray[i].xyz[0];
	//	vertexP[i].y = vertexArray[i].xyz[1];
	//	vertexP[i].z = vertexArray[i].xyz[2];
	//	vertexP[i].nx = vertexArray[i].nxyz[0];
	//	vertexP[i].ny = vertexArray[i].nxyz[1];
	//	vertexP[i].nz = vertexArray[i].nxyz[2];
	//	vertexP[i].u = tempTex.u;
	//	vertexP[i].v = tempTex.v;
	//}
	//glUnmapBuffer(GL_ARRAY_BUFFER);

	//// Specify how many attribute arrays we have in our VAO
	//glEnableVertexAttribArray(0); // Vertex coordinates
	//glEnableVertexAttribArray(1); // Normals
	//glEnableVertexAttribArray(2); // texCoords
	//// Specify how OpenGL should interpret the vertex buffer data:
	//// Attributes 0, 1, 2 (must match the lines above and the layout in the shader)
	//// Number of dimensions (3 means vec3 in the shader, 2 means vec2)
	//// Type GL_FLOAT
	//// Not normalized (GL_FALSE)
	//// Stride 8 (interleaved array with 8 floats per vertex)
	//// Array buffer offset 0, 3, 6 (offset into first vertex)
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
	//	sizeof(sBufferData), (void*)0); // xyz coordinates
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
	//	sizeof(sBufferData), (void*)(3 * sizeof(GLfloat))); // normals
	//glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE,
	//	sizeof(sBufferData), (void*)(6 * sizeof(GLfloat))); // texCoords

	//// Activate the index buffer
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexbuffer);
	//// Present our vertex indices to OpenGL
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER,
	//	(nrofTris)*sizeof(triangle), NULL, GL_STREAM_DRAW);

	//indexP = (triangle*)glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(triangle) * nrofTris,
	//	GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT | GL_MAP_UNSYNCHRONIZED_BIT);

	//for (int i = 0; i < nrofTris; i++) {
	//	indexP[i].index[0] = triangleArray[i].index[0];
	//	indexP[i].index[1] = triangleArray[i].index[1];
	//	indexP[i].index[2] = triangleArray[i].index[2];
	//}

	//glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);

	//// Deactivate (unbind) the VAO and the buffers again.
	//// Do NOT unbind the buffers while the VAO is still bound.
	//// The index buffer is an essential part of the VAO state.
	//glBindVertexArray(0);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void StaticMesh::load(std::string fileName) {
	//// read mesh from file
	//std::cout << "loading mesh..." << std::endl;

	//std::ifstream file(fileName, std::ios::in | std::ios::binary);

	//if (file.is_open()) {

	//	int bitCount = 0;

	//	// read the size of the arrays
	//	file.seekg(0);
	//	file.read((char*)&nrofVerts, sizeof(int));
	//	bitCount += sizeof(int);
	//	nrofVerts++;

	//	file.seekg(bitCount);
	//	file.read((char*)&nrofTris, sizeof(int));
	//	bitCount += sizeof(int);
	//	nrofTris++;

	//	vertexArray = new vertex[nrofVerts];
	//	triangleArray = new triangle[nrofTris];

	//	/*file.seekg(bitCount);
	//	file.read((char*)&edgeCap, sizeof(int));*/
	//	bitCount += sizeof(int); // Skip edgeCap

	//	// read mesh position
	//	file.seekg(bitCount);
	//	file.read((char*)&position[0], sizeof(float) * 3);
	//	bitCount += sizeof(float) * 3;

	//	// read mesh orientation
	//	file.seekg(bitCount);
	//	file.read((char*)&orientation[0], sizeof(float) * 16);
	//	bitCount += sizeof(float) * 16;

	//	// read vertecies
	//	//file.seekg(bitCount);
	//	//file.read((char*)&nrofVerts, sizeof(int));
	//	bitCount += sizeof(int); // skip nrofVerts

	//	file.seekg(bitCount);
	//	file.read((char*)&vertexArray[0], sizeof(vertex) * (nrofVerts));
	//	bitCount += sizeof(vertex) * (nrofVerts);

	//	// read triangles
	//	//file.seekg(bitCount);
	//	//file.read((char*)&nrofTris, sizeof(int));
	//	bitCount += sizeof(int); // skip nrofTris

	//	file.seekg(bitCount);
	//	file.read((char*)&triangleArray[0], sizeof(triangle) * (nrofTris));

	//	file.close();

	//	std::cout << "load complete" << std::endl;
	//}
	//else {
	//	std::cout << "could not open file" << std::endl;
	//}
}
