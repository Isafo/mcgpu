#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include "StaticMesh.h"
#include <glm/mat4x4.hpp> // glm::mat4

#include <stdio.h>  // For file input in soupReadOBJ()
#include <stdlib.h> // For malloc() and free()
#include <math.h>   // For sin() and cos() in soupCreateSphere()
#include <iomanip>

class gObject {
	public:
		char getOtype();

		void getPosition(glm::vec3& _Position);
		float* getPositionF();
		glm::vec3* getPositionV();
		void setPosition(glm::vec3* _Position);

		float* getTransformF();
		glm::mat4* getTransformM();

		void setRotation(glm::mat3* _Rotation);
		void render();

	protected:

		StaticMesh* shape;

		char oType; 
		glm::mat4 transform;
};