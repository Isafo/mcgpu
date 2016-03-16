#include "testGenerator.h"

#include <cmath>

testGenerator::testGenerator()
{
	octantStack.reserve(100);

	mcShader.createShader("shaders/MC.vert", "shaders/MC.frag", "shaders/MC.geom");

	volumeTex = glGetUniformLocation(mcShader.programID, "scalarField");
	triTable = glGetUniformLocation(mcShader.programID, "triTable");

	octantPos = glGetUniformLocation(mcShader.programID, "octantPos");
}

testGenerator::~testGenerator(){

}

void testGenerator::generate(Octant* _ot, DynamicMesh* _dm){

	//Generate scalar field ---------------------------------------

	_ot->voxelData = new unsigned char[_dm->voxelRes*_dm->voxelRes*_dm->voxelRes];


	for (int x = 0; x < _dm->voxelRes; ++x){
		for (int y = 0; y < _dm->voxelRes; ++y){
			for (int z = 0; z < _dm->voxelRes; ++z){
				if ((x > 75 && x < 200) && (y > 75 && y < 200) && (z > 75 && z < 200))
					_ot->voxelData[x + _dm->voxelRes*(y + _dm->voxelRes*z)] = 255;
				else
					_ot->voxelData[x + _dm->voxelRes*(y + _dm->voxelRes*z)] = 0;
			}
		}
	}
	
	
	// generate marching cubes on the GPU ------------------------------
	// bind mcShader
	glUseProgram(mcShader.programID);

	

	// bind textures
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, _dm->triTableTex);
	/*glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, _dm->edgeTableTex);*/
	
	// send scalar field as 3D texture to the GPU ---------------------
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_3D, _dm->voxelTex);
	//glTexImage3D(GL_TEXTURE_3D, 0, GL_RED, _dm->voxelRes, _dm->voxelRes, _dm->voxelRes, 0, GL_RED, GL_UNSIGNED_BYTE, _ot->voxelData);
	//std::cout << "hej";

	// send uniforms
	glUniform3fv(octantPos, 1, &_ot->pos[0]);
	glUniform1i(volumeTex, 0);
	glUniform1i(triTable, 1);
	//glUniform1i(edgeTable, 2);

	//start rendering with transform feedback
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, _dm->vertexbuffer);
	//glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, _dm->indexbuffer);

	glBindVertexArray(_dm->singlePointvao);
	
	GLuint qid;
	glGenQueries(1, &qid);

	glBeginQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN, qid);

	glEnable(GL_RASTERIZER_DISCARD);
	glBeginTransformFeedback(GL_TRIANGLES);
	glDrawArraysInstanced(GL_POINTS, 0, 1, std::pow(_dm->voxelRes, 3));
	glEndTransformFeedback();
	glDisable(GL_RASTERIZER_DISCARD);

	glEndQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN);
	GLuint nprimitives;
	glGetQueryObjectuiv(qid, GL_QUERY_RESULT, &nprimitives);


	GLfloat feedback[1000];
	glGetBufferSubData(GL_TRANSFORM_FEEDBACK_BUFFER, 0, sizeof(feedback), feedback);
	for (int i = 0; i < 1000; i = i + 4 ) {
		std::cout << feedback[i] << ", " <<
					 feedback[i + 1] << ", " << 
					 feedback[i + 2] << ", " << 
					 feedback[i + 3] << ", " << std::endl;
	}


	glBindVertexArray(0);

}
