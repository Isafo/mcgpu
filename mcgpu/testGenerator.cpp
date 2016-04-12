#include "testGenerator.h"

#include <cmath>

testGenerator::testGenerator()
{
	octantStack.reserve(100);

	//non optimized MC shader
	//mcShader.createShader("shaders/MC.vert", "shaders/MC.frag", "shaders/MC.geom", "vertexPosition", "vertexNormal");

	//volumeTex1 = glGetUniformLocation(mcShader.programID, "scalarField");
	//triTable1 = glGetUniformLocation(mcShader.programID, "triTable");

	//octantPos = glGetUniformLocation(mcShader.programID, "octantPos");

	// MC pass 1 shader
	mc1Shader.createShader("shaders/MC1.vert", "shaders/MC1.frag", "shaders/MC1.geom", "vertexPosition");
	volumeTex1 = glGetUniformLocation(mc1Shader.programID, "scalarField");
	triTable1 = glGetUniformLocation(mc1Shader.programID, "triTable");
	octantPos1 = glGetUniformLocation(mc1Shader.programID, "octantPos");

	// MC pass 2 shader
	mc2Shader.createShader("shaders/MC2.vert", "shaders/MC2.frag", "shaders/MC2.geom");
	volumeTex2 = glGetUniformLocation(mc2Shader.programID, "scalarField");
	triTable2 = glGetUniformLocation(mc2Shader.programID, "triTable");
	edgeTable2 = glGetUniformLocation(mc2Shader.programID, "edgeTable");
	octantPos2 = glGetUniformLocation(mc1Shader.programID, "octantPos");
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
	

	// bind textures
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, _dm->triTableTex);
	/*glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, _dm->edgeTableTex);*/
	// send scalar field as 3D texture to the GPU ---------------------
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_3D, _dm->voxelTex);
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RED, _dm->voxelRes, _dm->voxelRes, _dm->voxelRes, 0, GL_RED, GL_UNSIGNED_BYTE, _ot->voxelData);

	//FIRST PASS - march all voxels and output those that needed tris to
	// VBO "nonEmptyCellsBuffer" ---------------------------------------------------------------
	glUseProgram(mc1Shader.programID);

	// send uniforms
	glUniform3fv(octantPos1, 1, &_ot->pos[0]);
	glUniform1i(volumeTex1, 0);
	glUniform1i(triTable1, 1);
	//glUniform1i(edgeTable, 2);

	//start rendering with transform feedback
	//glBindBufferRange(GL_TRANSFORM_FEEDBACK_BUFFER, 0, _dm->vertexbuffer, 0, 10000*sizeof(dBufferData));
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, _dm->nonEmptyCellsBuffer);
	//glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, _dm->indexbuffer);

	glBindVertexArray(_dm->singlePointvao);
	
	GLuint qid;
	glGenQueries(1, &qid);

	glBeginQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN, qid);

	glEnable(GL_RASTERIZER_DISCARD);
	glBeginTransformFeedback(GL_POINTS);
	glDrawArraysInstanced(GL_POINTS, 0, 1, std::pow(_dm->voxelRes - 2, 3));
	glEndTransformFeedback();
	glDisable(GL_RASTERIZER_DISCARD);

	//glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, 0);

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
	
	//SECOND PASS - march voxels that needed tris and output edges that needed verts to
	// texture "nonEmptyEdgesBuffer" ---------------------------------------------------------------
	glUseProgram(mc2Shader.programID);

	// bind textures
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, _dm->edgeTableTex);
	// send scalar field as 3D texture to the GPU ---------------------


	// send uniforms
	glUniform3fv(octantPos2, 1, &_ot->pos[0]);
	glUniform1i(volumeTex2, 0);
	glUniform1i(triTable2, 1);
	glUniform1i(edgeTable2, 2);

	glViewport(0, 0, 32, 32);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _dm->tmpFbo);
	//glClear(GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(_dm->nonEmptyCellsArray);
	//TODO: determine number of points to draw properly
	glDrawArrays(GL_POINTS, 0, nprimitives);
	
	GLint pixels1[32 * 32 * 32];
	GLubyte pixels[32*32*32*4];
	
	glBindTexture(GL_TEXTURE_3D, _dm->edgeTex);
	glGetTexImage(GL_TEXTURE_3D, 0, GL_RGBA, GL_UNSIGNED_BYTE, &pixels[0]);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glViewport(0, 0, 1920, 1080);

	glBindVertexArray(0);

}
