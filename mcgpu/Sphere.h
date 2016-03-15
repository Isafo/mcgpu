#pragma once
#include "gObject.h"
class Sphere : public gObject {
public:

	Sphere();
	
	// Creates a sphere  
	Sphere(float _x, float _y, float _z, float _rad);
	~Sphere(void);

	float getRadius(){return radius;}

private:

	float radius;
	void clean();
};