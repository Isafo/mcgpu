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
	uID_volumeTex1 = glGetUniformLocation(mc1Shader.programID, "scalarField");
	uID_triTable1 = glGetUniformLocation(mc1Shader.programID, "triTable");
	uID_octantPos1 = glGetUniformLocation(mc1Shader.programID, "octantPos");

	// MC pass 2 shader
	mc2Shader.createShader("shaders/MC2.vert", "shaders/MC2.frag", "shaders/MC2.geom");
	uID_volumeTex2 = glGetUniformLocation(mc2Shader.programID, "scalarField");
	uID_triTable2 = glGetUniformLocation(mc2Shader.programID, "triTable");
	uID_edgeTable2 = glGetUniformLocation(mc2Shader.programID, "edgeTable");
	uID_octantPos2 = glGetUniformLocation(mc2Shader.programID, "octantPos");
	//// MC pass 3 shader
	mc3Shader.createShader("shaders/MC3.vert", "shaders/MC3.frag", "shaders/MC3.geom", "vertexPosition", "vertexNormal");
	uID_volumeTex3 = glGetUniformLocation(mc3Shader.programID, "scalarField");
	uID_octantPos3 = glGetUniformLocation(mc3Shader.programID, "octantPos");
	uID_edgeTex3 = glGetUniformLocation(mc3Shader.programID, "edgeTex");

	// MC pass 4 shader
	mc4Shader.createShader("shaders/MC4.vert", "shaders/MC4.frag", "shaders/MC4.geom");
	uID_neededEdges = glGetUniformLocation(mc4Shader.programID, "neededEdges");
	uID_octantPos4 = glGetUniformLocation(mc4Shader.programID, "octantPos");

	// MC pass 5 shader
	mc5Shader.createShader("shaders/MC5.vert", "shaders/MC5.frag", "shaders/MC5.geom", "index");
	uID_volumeTex5 = glGetUniformLocation(mc4Shader.programID, "scalarField");
	uID_triTable5 = glGetUniformLocation(mc1Shader.programID, "triTable");
	uID_vertIndices5 = glGetUniformLocation(mc1Shader.programID, "vIndices");
	uID_octantPos5 = glGetUniformLocation(mc4Shader.programID, "octantPos");

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
	// VBO "nonEmptyCellsBuffer" =======================================================================================
	glUseProgram(mc1Shader.programID);

	// send uniforms
	glUniform3fv(uID_octantPos1, 1, &_ot->pos[0]);
	glUniform1i(uID_volumeTex1, 0);
	glUniform1i(uID_triTable1, 1);
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

	// debug code <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<//<<
	std::cout << "first pass ========================" << std::endl;													//<<
	GLuint nprimitives;																								  //<<
	glGetQueryObjectuiv(qid, GL_QUERY_RESULT, &nprimitives);															//<<
	_dm->nrofVerts = nprimitives;																						//<<
																														//<<
	GLfloat feedback[5000];
	glGetBufferSubData(GL_TRANSFORM_FEEDBACK_BUFFER, 0, sizeof(feedback), feedback);										//<<
	for (int i = 0; i < 3*6*3; i = i + 3 ) {																				//<<
		std::cout << feedback[i] << ", " <<																				//<<
			feedback[i + 1] << ", " <<																					//<<
			feedback[i + 2] << ", " << std::endl;																	   //<<
			if (i % 9 == 0)																							   //<<
				std::cout <<  "----------" << std::endl;															   //<<
																													   //<<
	}																												   //<<
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


	//SECOND PASS - march voxels that needed tris and output edges that needed verts to
	// texture "nonEmptyEdgesBuffer" =======================================================================================
	glUseProgram(mc2Shader.programID);

	// bind textures
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, _dm->edgeTableTex);

	// send uniforms
	glUniform3fv(uID_octantPos2, 1, &_ot->pos[0]);
	glUniform1i(uID_volumeTex2, 0);
	glUniform1i(uID_triTable2, 1);
	glUniform1i(uID_edgeTable2, 2);

	//set viewport and bind fbo
	glViewport(0, 0, 32, 32);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _dm->tmpFbo);
	GLfloat cl[] = { 0.0, 0.0, 0.0, 0.0 };
	glClearBufferfv(GL_COLOR, GL_DRAW_BUFFER0, cl);

	glBindVertexArray(_dm->nonEmptyCellsArray);
	//TODO: determine number of points to draw properly
	//TODO: look at creating a TFO and using glDrawTransformFeedback
	glDrawArrays(GL_POINTS, 0, nprimitives);

	// debug code <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<//<<
	std::cout << "second pass ========================" << std::endl;													//<<
	GLint pixelData[32 * 32 * 32 * 3];
	pixelData[0] = 0;
	glBindTexture(GL_TEXTURE_3D, _dm->edgeTex);
	glGetTexImage(GL_TEXTURE_3D, 0, GL_RGB, GL_INT, pixelData);

	for (int i = 0; i < 3 * 32 * 32 * 32; i = i + 3) {																				//<<
		std::cout << pixelData[i] << ", " <<																				//<<
			pixelData[i + 1] << ", " <<																						//<<
			pixelData[i + 2] << ", " << std::endl;																			//<<
		if (i % 9 == 0)																										//<<
			std::cout << "----------" << std::endl;																			//<<
																															//<<
	}//<<
	glBindTexture(GL_TEXTURE_3D, 0);
	 //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	

	////THIRD PASS - march voxels that needed tris and use the texture containing info about
	////which edges that need verts to calc these verts.
	////=======================================================================================
	glUseProgram(mc3Shader.programID);

	// bind textures
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_3D, _dm->edgeTex);
	GLubyte pixels[32 * 32 * 32 * 3];
	glGetTexImage(GL_TEXTURE_3D, 0, GL_RGB, GL_UNSIGNED_BYTE, &pixels[0]);
	
	// send uniforms
	glUniform3fv(uID_octantPos3, 1, &_ot->pos[0]);
	glUniform1i(uID_volumeTex3, 0);
	glUniform1i(uID_edgeTex3, 3);

	//start rendering with transform feedback
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, _dm->vertexbuffer);

	glBeginQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN, qid);

	glEnable(GL_RASTERIZER_DISCARD);
	glBeginTransformFeedback(GL_POINTS);
	glDrawArrays(GL_POINTS, 0, nprimitives);
	glEndTransformFeedback();
	glDisable(GL_RASTERIZER_DISCARD);

	glEndQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN);

	// debug code <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<//<<
	std::cout << "third pass ========================" << std::endl;													//<<
	GLuint nprimitives1;																									//<<
	glGetQueryObjectuiv(qid, GL_QUERY_RESULT, &nprimitives1);																//<<
	_dm->nrofVerts = nprimitives1;																							//<<
																															//<<
	GLfloat feedback1[5000];																								//<<
	glGetBufferSubData(GL_TRANSFORM_FEEDBACK_BUFFER, 0, sizeof(feedback1), feedback1);										//<<
	for (int i = 0; i < 3 * 6 * 3; i = i + 3) {																				//<<
		std::cout << feedback1[i] << ", " <<																				//<<
			feedback1[i + 1] << ", " <<																						//<<
			feedback1[i + 2] << ", " << std::endl;																			//<<
		if (i % 9 == 0)																										//<<
			std::cout << "----------" << std::endl;																			//<<
																															//<<
	}																														//<<
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

	//FOURTH PASS ============================================================================


	//FIFTH PASS =============================================================================


	//Generation complete =====================================================================

	glViewport(0, 0, 1920, 1080);

	glBindVertexArray(0);

}
