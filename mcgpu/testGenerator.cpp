#include "testGenerator.h"

#include <cmath>

testGenerator::testGenerator()
{
	octantStack.reserve(100);

	//non optimized MC shader
	mcShader.createShader("shaders/MC.vert", "shaders/MC.frag", "shaders/MC.geom");

	volumeTex = glGetUniformLocation(mcShader.programID, "scalarField");
	triTable = glGetUniformLocation(mcShader.programID, "triTable");

	octantPos = glGetUniformLocation(mcShader.programID, "octantPos");

	// MC pass 1 shader
	//mc1Shader.createShader("shaders/MC1.vert", "shaders/MC1.frag", "shaders/MC1.geom");
	//volumeTex = glGetUniformLocation(mc1Shader.programID, "scalarField");
	//triTable = glGetUniformLocation(mc1Shader.programID, "triTable");

	//octantPos = glGetUniformLocation(mc1Shader.programID, "octantPos");

	// MC pass 2 shader
	//
	//something...
	//
	
	// MC pass 3 shader
	//
	//something...
	//

	// MC pass 4 shader
	//
	//something...
	//

	// MC pass 5 shader
	//
	//something...
	//

}

testGenerator::~testGenerator(){

}

void testGenerator::generate(Octant* _ot, DynamicMesh* _dm){

	//Generate scalar field ---------------------------------------

	_ot->voxelData = new unsigned char[_dm->voxelRes*_dm->voxelRes*_dm->voxelRes];


	for (int x = 0; x < _dm->voxelRes; ++x){
		for (int y = 0; y < _dm->voxelRes; ++y){
			for (int z = 0; z < _dm->voxelRes; ++z){
				/*if ((x > _dm->voxelRes / 4 && x < (_dm->voxelRes * 3) / 4) && (y > _dm->voxelRes / 4 && y < (_dm->voxelRes * 3) / 4) && (z > _dm->voxelRes / 4 && z < (_dm->voxelRes * 3) / 4))
					_ot->voxelData[x + _dm->voxelRes*(y + _dm->voxelRes*z)] = 255;
				else
					_ot->voxelData[x + _dm->voxelRes*(y + _dm->voxelRes*z)] = 0;
				
				if (x == _dm->voxelRes / 4 && y % 4 == 0 && (y > _dm->voxelRes / 4 && y < (_dm->voxelRes * 3) / 4) && (z > _dm->voxelRes / 4 && z < (_dm->voxelRes * 3) / 4))
					_ot->voxelData[x + _dm->voxelRes*(y + _dm->voxelRes*z)] = 255;*/

				if (y < (_dm->voxelRes / 2) + sin(100.0f*static_cast<float>(x) / _dm->voxelRes) + sin(100.0f*static_cast<float>(z) / _dm->voxelRes) || 
					( (x % 6)  == 0 && (z % 6)  == 0 && y < (_dm->voxelRes / 2) + 10 ))
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
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RED, _dm->voxelRes, _dm->voxelRes, _dm->voxelRes, 0, GL_RED, GL_UNSIGNED_BYTE, _ot->voxelData);
	//std::cout << "hej";

	// send uniforms
	glUniform3fv(octantPos, 1, &_ot->pos[0]);
	glUniform1i(volumeTex, 0);
	glUniform1i(triTable, 1);
	//glUniform1i(edgeTable, 2);

	//start rendering with transform feedback
	//glBindBufferRange(GL_TRANSFORM_FEEDBACK_BUFFER, 0, _dm->vertexbuffer, 0, 10000*sizeof(dBufferData));
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, _dm->vertexbuffer);
	//glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, _dm->indexbuffer);

	glBindVertexArray(_dm->singlePointvao);
	
	GLuint qid;
	glGenQueries(1, &qid);

	glBeginQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN, qid);

	glEnable(GL_RASTERIZER_DISCARD);
	glBeginTransformFeedback(GL_TRIANGLES);
	glDrawArraysInstanced(GL_POINTS, 0, 1, std::pow(_dm->voxelRes - 2, 3));
	glEndTransformFeedback();
	glDisable(GL_RASTERIZER_DISCARD);

	glEndQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN);
	GLuint nprimitives;
	glGetQueryObjectuiv(qid, GL_QUERY_RESULT, &nprimitives);
	_dm->nrofVerts = nprimitives;

	GLfloat feedback[5000];
	glGetBufferSubData(GL_TRANSFORM_FEEDBACK_BUFFER, 0, sizeof(feedback), feedback);
	for (int i = 0; i < 3*6*3; i = i + 3 ) {
		std::cout << feedback[i] << ", " <<
			feedback[i + 1] << ", " <<
			feedback[i + 2] << ", " << std::endl;
			if (i % 9 == 0)
				std::cout <<  "----------" << std::endl;
		
	}
	
	glBindVertexArray(0);

}
