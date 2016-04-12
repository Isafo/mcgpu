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
	GLint volumeTex;
	GLint triTable;
	GLint octantPos;

	Shader mc1Shader;
	GLint volumeTex1;
	GLint triTable1;
	GLint octantPos1;

	Shader mc2Shader;
	GLint volumeTex2;
	GLint triTable2;
	GLint edgeTable2;
	GLint octantPos2;

	Shader mc3Shader;
	Shader mc4Shader;
	Shader mc5Shader;



};

