#pragma once
#include "Octant.h"
#include "DynamicMesh.h"
#include "Shader.h"
class testGenerator
{
public:
	testGenerator();
	~testGenerator();

	void generate(Octant* _ot, DynamicMesh* _dm);

private:

	std::vector<octantStackElement> octantStack;

	Shader mcShader;

	Shader mc1Shader;
	Shader mc2Shader;
	Shader mc3Shader;
	Shader mc4Shader;
	Shader mc5Shader;

	//GLint locationDims;
	//GLint locationVposition;

	GLint volumeTex;
	GLint triTable;
	GLint edgeTable;

	GLint octantPos;
	//GLint isoValue;


};

