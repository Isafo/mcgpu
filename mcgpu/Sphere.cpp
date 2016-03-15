#include "Sphere.h"

Sphere::Sphere()
{

}

Sphere::Sphere(float _x, float _y, float _z, float _rad) 
{
	oType = 'S';

	transform = glm::mat4(1.0f);
	transform[3][0] = _x;
	transform[3][1] = _y;
	transform[3][2] = _z;

	radius = _rad;

	float GOOD_M_PI = 3.14159265358979323846;
	int i, j, base, i0;
	float x, y, z, R;
	double theta, phi;
	int vsegs, hsegs;
	int stride = 6; // number of variables in a vertex (pos, normal, texture (ts))

	// Delete any previous content in the TriangleSoup object
	shape = new StaticMesh;

	int segments = 32;

	vsegs = segments;
	if (vsegs < 2) vsegs = 2;
	hsegs = vsegs * 2;
	shape->nrofVerts = 1 + (vsegs - 1) * (hsegs + 1) + 1; // top + middle + bottom
	shape->nrofTris = hsegs + (vsegs - 2) * hsegs * 2 + hsegs; // top + middle + bottom
	shape->vertexArray = new vertex*[1]; shape->vertexArray[0] = new vertex[shape->nrofVerts];
	shape->triangleArray = new triangle*[1]; shape->triangleArray[0] = new triangle[shape->nrofTris];

	// The vertex array: 3D xyz, 3D normal, 2D st (8 floats per vertex)
	// First vertex: top pole (+z is "up" in object local coords)
	shape->vertexArray[0][0].xyz[0] = 0.0f;
	shape->vertexArray[0][0].xyz[1] = 0.0f;
	shape->vertexArray[0][0].xyz[2] = radius;
	shape->vertexArray[0][0].nxyz[0] = 0.0f;
	shape->vertexArray[0][0].nxyz[1] = 0.0f;
	shape->vertexArray[0][0].nxyz[2] = 1.0f;
	//vertexArray[6] = 0.5f;
	//vertexArray[7] = 1.0f;
	// Last vertex: bottom pole
	base = (shape->nrofVerts - 1);
	shape->vertexArray[0][base].xyz[0] = 0.0f;
	shape->vertexArray[0][base].xyz[1] = 0.0f;
	shape->vertexArray[0][base].xyz[2] = -radius;
	shape->vertexArray[0][base].nxyz[0] = 0.0f;
	shape->vertexArray[0][base].nxyz[1] = 0.0f;
	shape->vertexArray[0][base].nxyz[2] = -1.0f;
	//vertexArray[base] = 0.5f;
	//vertexArray[base] = 0.0f;
	// All other vertices:
	// vsegs-1 latitude rings of hsegs+1 vertices each
	// (duplicates at texture seam s=0 / s=1)
	for (j = 0; j<vsegs - 1; j++) { // vsegs-1 latitude rings of vertices
		theta = (double)(j + 1) / vsegs*GOOD_M_PI;
		z = cos(theta);
		R = sin(theta);
		for (i = 0; i <= hsegs; i++) { // hsegs+1 vertices in each ring (duplicate for texcoords)
			phi = (double)i / hsegs*2.0*GOOD_M_PI;
			x = R*cos(phi);
			y = R*sin(phi);
			base = (1 + j*(hsegs + 1) + i);
			shape->vertexArray[0][base].xyz[0] = radius*x;
			shape->vertexArray[0][base].xyz[1] = radius*y;
			shape->vertexArray[0][base].xyz[2] = radius*z;
			shape->vertexArray[0][base].nxyz[0] = x;
			shape->vertexArray[0][base].nxyz[1] = y;
			shape->vertexArray[0][base].nxyz[2] = z;
			//vertexArray[base] = (float)i / hsegs;
			//vertexArray[base] = 1.0f - (float)(j + 1) / vsegs;
		}
	}

	// The index array: triplets of integers, one for each triangle
	// Top cap
	for (i = 0; i<hsegs; i++) {
		shape->triangleArray[0][i].index[0] = 0;
		shape->triangleArray[0][i].index[1] = 1 + i;
		shape->triangleArray[0][i].index[2] = 2 + i;
	}
	// Middle part (possibly empty if vsegs=2)
	for (j = 0; j<vsegs - 2; j++) {
		for (i = 0; i<hsegs; i++) {
			base = hsegs + 2 * (j*hsegs + i);
			i0 = 1 + j*(hsegs + 1) + i;
			shape->triangleArray[0][base].index[0] = i0;
			shape->triangleArray[0][base].index[1] = i0 + hsegs + 1;
			shape->triangleArray[0][base].index[2] = i0 + 1;

			shape->triangleArray[0][base + 1].index[0] = i0 + 1;
			shape->triangleArray[0][base + 1].index[1] = i0 + hsegs + 1;
			shape->triangleArray[0][base + 1].index[2] = i0 + hsegs + 2;
		}
	}
	// Bottom cap
	base = hsegs + 2 * (vsegs - 2)*hsegs;
	for (i = 0; i<hsegs; i++) {
		shape->triangleArray[0][base + i].index[0] = shape->nrofVerts - 1;
		shape->triangleArray[0][base + i].index[1] = shape->nrofVerts - 2 - i;
		shape->triangleArray[0][base + i].index[2] = shape->nrofVerts - 3 - i;
	}

	// Generate one vertex array object (VAO) and bind it
	glGenVertexArrays(1, &(shape->vao));
	glBindVertexArray(shape->vao);

	// Generate two buffer IDs
	glGenBuffers(1, &shape->vertexbuffer);
	glGenBuffers(1, &shape->indexbuffer);

	// Activate the vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, shape->vertexbuffer);
	// Present our vertex coordinates to OpenGL
	glBufferData(GL_ARRAY_BUFFER,
		stride * shape->nrofVerts * sizeof(GLfloat), (*shape->vertexArray), GL_STATIC_DRAW);
	// Specify how many attribute arrays we have in our VAO
	glEnableVertexAttribArray(0); // Vertex coordinates
	glEnableVertexAttribArray(1); // Normals
	//glEnableVertexAttribArray(2); // Texture coordinates
	// Specify how OpenGL should interpret the vertex buffer data:
	// Attributes 0, 1, 2 (must match the lines above and the layout in the shader)
	// Number of dimensions (3 means vec3 in the shader, 2 means vec2)
	// Type GL_FLOAT
	// Not normalized (GL_FALSE)
	// Stride 8 (interleaved array with 8 floats per vertex)
	// Array buffer offset 0, 3, 6 (offset into first vertex)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
		stride * sizeof(GLfloat), (void*)0); // xyz coordinates
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
		stride * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat))); // normals
	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
	//	stride * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat))); // texcoords

	// Activate the index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, shape->indexbuffer);
	// Present our vertex indices to OpenGL
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		3 * shape->nrofTris*sizeof(GLuint), (*shape->triangleArray), GL_STATIC_DRAW);

	// Deactivate (unbind) the VAO and the buffers again.
	// Do NOT unbind the buffers while the VAO is still bound.
	// The index buffer is an essential part of the VAO state.
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


void Sphere::clean() 
{
	

	if (glIsVertexArray(shape->vao)) {
		glDeleteVertexArrays(1, &shape->vao);
	}
	shape->vao = 0;

	if (glIsBuffer(shape->vertexbuffer)) {
		glDeleteBuffers(1, &shape->vertexbuffer);
	}
	shape->vertexbuffer = 0;

	if (glIsBuffer(shape->indexbuffer)) {
		glDeleteBuffers(1, &shape->indexbuffer);
	}
	shape->indexbuffer = 0;
}


Sphere::~Sphere(void) {
	std::cout << "A sphere has died" << std::endl;
	delete[] (*shape->vertexArray);
	delete[] (*shape->triangleArray);

	delete[] shape->vertexArray;
	delete[] shape->triangleArray;

	clean();
}



