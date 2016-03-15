#include "gObject.h"
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> //glm::make:mat4

char gObject::getOtype()
{ 
	return oType; 
}

void gObject::getPosition(glm::vec3& _Position)
{ 
	_Position[0] = transform[4][0];
	_Position[1] = transform[4][1];
	_Position[2] = transform[4][2];
}

float* gObject::getPositionF()
{
	return &transform[3][0];
}

glm::vec3* gObject::getPositionV()
{
	return (glm::vec3*)(&transform[3]);
}

void gObject::setPosition(glm::vec3* _Position)
{ 
	transform[0][3] = (*_Position)[0];
	transform[1][3] = (*_Position)[1];
	transform[2][3] = (*_Position)[2];
}

float* gObject::getTransformF()
{ 
	return &transform[0][0]; 
}

glm::mat4* gObject::getTransformM()
{
	return &transform;
}
void gObject::setRotation(glm::mat3* _Rotation)
{ 
	transform[0][0] = (*_Rotation)[0][0];
	transform[0][1] = (*_Rotation)[0][1];
	transform[0][2] = (*_Rotation)[0][2];

	transform[1][0] = (*_Rotation)[1][0];
	transform[1][1] = (*_Rotation)[1][1];
	transform[1][2] = (*_Rotation)[1][2];

	transform[2][0] = (*_Rotation)[2][0];
	transform[2][1] = (*_Rotation)[2][1];
	transform[2][2] = (*_Rotation)[2][2];
}
void gObject::render()
{ 
	shape->render(); 
}