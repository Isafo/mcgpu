#pragma once
#include <glm/mat4x4.hpp> // glm::mat4
class Camera
{
public:
	Camera();
	~Camera();

	void getPosition(glm::vec3& _Position);
	float* getPositionF();

	float* getTransformF();
	glm::mat4* getTransformM();
	float* getPerspective();

	void setPosition(glm::vec3* _Position);

	void setTransform(glm::mat4* _Transform);
	void setPerspective(glm::mat4* _Perspective);

private:
	glm::mat4 transform;
	glm::mat4 perspective;
};

