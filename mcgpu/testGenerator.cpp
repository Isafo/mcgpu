#include "testGenerator.h"

#include <cmath>

testGenerator::testGenerator()
{
	octantStack.reserve(100);

	mcShader.createShader("shaders/MC.vert", "shaders/MC.frag", "shaders/MC.geom");

	volumeTex = glGetUniformLocation(mcShader.programID, "scalarField");
	triTable = glGetUniformLocation(mcShader.programID, "triTable");
	edgeTable = glGetUniformLocation(mcShader.programID, "edgeTable");

	octantPos = glGetUniformLocation(mcShader.programID, "octantPos");
}

testGenerator::~testGenerator(){

}

void testGenerator::generate(Octant* _ot, DynamicMesh* _dm){

	//Generate scalar field ---------------------------------------

	_ot->voxelData = new unsigned char**[256];

	for (int i = 0; i < 256; ++i){
		_ot->voxelData[i] = new unsigned char*[256];
	}

	for(int i = 0; i < 256; ++i)
		for (int j = 0; j < 256; ++j){
			_ot->voxelData[i][j] = new unsigned char[256];
		}

	for (int x = 0; x < 256; ++x){
		for (int y = 0; y < 256; ++y){
			for (int z = 0; z < 256; ++z){
				if ((x > 75 && x < 200) && (y > 75 && y < 200) && (z > 75 && z < 200))
					_ot->voxelData[x][y][z] = 255;
				else
					_ot->voxelData[x][y][z] = 0;
			}
		}
	}
	
	// send scalar field as 3D texture to the GPU ---------------------
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_3D, _dm->voxelTex);
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RED, _dm->voxelRes, _dm->voxelRes, _dm->voxelRes, 0, GL_RED, GL_UNSIGNED_BYTE, _ot->voxelData);

	// generate marching cubes on the GPU ------------------------------
	// bind mcShader
	glUseProgram(mcShader.programID);

	// bind textures
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, _dm->triTableTex);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, _dm->edgeTableTex);
	
	// send uniforms
	glUniform3fv(octantPos, 1, &_ot->pos[0]);
	glUniform1i(volumeTex, 0);
	glUniform1i(triTable, 1);
	glUniform1i(edgeTable, 2);

	//start rendering with transform feedback
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, _dm->vao);
	glBindVertexArray(_dm->singlePointvao);

	glEnable(GL_RASTERIZER_DISCARD);
	glBeginTransformFeedback(GL_TRIANGLES);
	glDrawArraysInstanced(GL_POINTS, 0, 1, std::pow(_dm->voxelRes, 3));
	glEndTransformFeedback();
	glDisable(GL_RASTERIZER_DISCARD);

	glBindVertexArray(0);


}
