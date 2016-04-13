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
	GLint uID_volumeTex1;
	GLint uID_triTable1;
	GLint uID_octantPos1;

	Shader mc2Shader;
	GLint uID_volumeTex2;
	GLint uID_triTable2;
	GLint uID_edgeTable2;
	GLint uID_octantPos2;

	Shader mc3Shader;
	GLint uID_volumeTex3;
	GLint uID_octantPos3;
	GLint uID_edgeTex3;

	Shader mc4Shader;
	Shader mc5Shader;



};

