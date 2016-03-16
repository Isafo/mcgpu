#pragma once
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm\vec3.hpp>
#include <glm\vec2.hpp>
class Camera
{
public:
	Camera();
	~Camera();

	void getPosition(glm::vec3& _Position);
	float* getPositionF();
	glm::vec3* getDirection();

	float* getTransformF();
	glm::mat4* getTransformM();
	float* getPerspective();

	void translate(glm::vec3* _Translation);

	void setPosition(glm::vec3* _Position);

	void setTransform(glm::mat4* _Transform);
	void setPerspective(glm::mat4* _Perspective);

	void updateRot();

	glm::vec2 mouse;

private:
	glm::vec3 direction;

	glm::mat4 transform;
	glm::mat4 perspective;
};

