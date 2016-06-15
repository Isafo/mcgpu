#pragma once
#include "GL/glew.h"
#include <iomanip>

#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4

//! Data structure containing the coordinates and normal coordinates of a vertex, aswell as a pointer to an adjacent face
struct vertex {
	GLfloat xyz[3];
	GLfloat nxyz[3];
};

//! Data structure containing three indices of the vertexArray that make a certain triangle. Points to one edge in the triangle
struct triangle {
	GLuint index[3];
};

class Mesh
{

	friend class Sphere;

public:
	Mesh();
	~Mesh();

	glm::vec3* getPosition(){ return &position; };
	glm::mat4* getOrientation(){ return &orientation; };
	void getPosition(float* vec){ vec[0] = position[0]; vec[1] = position[1]; vec[2] = position[2]; };
	void getOrientation(float* mat);


	void setPosition(glm::vec3* p) { position = *p;}
	//void setOrientation(float* o) { std::copy(o, o + 16, orientation); }

	virtual void render() = 0;
	virtual void createBuffers() = 0;

	GLuint vao; // Vertex array object, the main handle for geometry
	GLuint vertexbuffer; // Buffer ID to bind to GL_ARRAY_BUFFER
	GLuint indexbuffer;  // Buffer ID to bind to GL_ELEMENT_ARRAY_BUFFER

	int nrofVerts;

protected:

	//array that stres all vertices of the mesh
	vertex** vertexArray;
	

	//Array that stores all triangles of the mesh
	triangle** triangleArray;
	int nrofTris;

	glm::vec3 position;
	glm::mat4 orientation;


};

