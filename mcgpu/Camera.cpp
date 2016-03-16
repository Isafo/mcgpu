#include "Camera.h"
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> //glm::make:mat4


Camera::Camera()
{
	transform = glm::mat4(1.0f);
	perspective = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.f);
	direction = glm::vec3(-transform[0][2], -transform[1][2], -transform[2][2] );

	mouse = glm::vec2(0.0f, 0.0f);
}


Camera::~Camera()
{

}

void Camera::getPosition(glm::vec3& _Position)
{
	_Position[0] = transform[3][0];
	_Position[1] = transform[3][1];
	_Position[2] = transform[3][2];
}

float* Camera::getPositionF()
{
	return &transform[3][0];
}

glm::vec3* Camera::getDirection()
{
	return &direction;
}

float* Camera::getTransformF()
{
	return &transform[0][0];
}
glm::mat4* Camera::getTransformM()
{
	return &transform;
}

float* Camera::getPerspective()
{
	return &perspective[0][0];
}

void Camera::translate(glm::vec3* _Translation)
{
	transform[3][0] += (*_Translation)[0];
	transform[3][1] += (*_Translation)[1];
	transform[3][2] += (*_Translation)[2];
}


void Camera::setPosition(glm::vec3* _Position)
{
	transform[3][0] = (*_Position)[0];
	transform[3][1] = (*_Position)[1];
	transform[3][2] = (*_Position)[2];
	
}

void Camera::setTransform(glm::mat4* _Transform)
{
	transform = *_Transform;
}

void Camera::setPerspective(glm::mat4* _Perspective)
{
	perspective = *_Perspective;
}

void Camera::updateRot()
{
	glm::mat4 rot;
	glm::vec3 up = glm::cross(direction, glm::vec3(0.0f, 0.0f, -1.0f));
	glm::normalize(up);
	float a = glm::dot(direction, glm::vec3(0.0f, 0.0f, -1.0f));
	rot = glm::rotate(rot, acos(a), up);

	transform[0][0] = rot[0][0]; transform[0][1] = rot[0][1]; transform[0][2] = rot[0][2];
	transform[1][0] = rot[1][0]; transform[1][1] = rot[1][1]; transform[1][2] = rot[1][2];
	transform[2][0] = rot[2][0]; transform[2][1] = rot[2][1]; transform[2][2] = rot[2][2];


}