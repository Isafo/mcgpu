#include "testGenerator.h"

#include <cmath>

testGenerator::testGenerator()
{
	octantStack.reserve(100);

	scalarGen.createShader("shaders/genScalarfield.vert", "shaders/genScalarfield.frag", "shaders/genScalarfield.geom");
	mcShader.createTransformShader("shaders/MC.vert", "shaders/MC.frag", "shaders/MC.geom");

	volumeTex = glGetUniformLocation(mcShader.programID, "scalarField");
	triTable = glGetUniformLocation(mcShader.programID, "triTable");

	octantPos = glGetUniformLocation(mcShader.programID, "octantPos");
}

testGenerator::~testGenerator(){

}

void testGenerator::generate(Octant* _ot, DynamicMesh* _dm){

	//Generate scalar field ---------------------------------------
	glUseProgram(scalarGen.programID);
	
	glViewport(0, 0, 32, 32);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _dm->tmpFbo);
	glBindVertexArray(_dm->singlePointvao);
	
	glDrawArraysInstanced(GL_POINTS, 0, 1, std::pow(_dm->voxelRes, 2));

	GLubyte pixels[32 * 32 * 32];

	glBindTexture(GL_TEXTURE_3D, _dm->voxelTex);
	glGetTexImage(GL_TEXTURE_3D, 0, GL_RED, GL_UNSIGNED_BYTE, &pixels[0]);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glViewport(0, 0, 1920, 1080);
	
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

	// DEBUG
	//GLfloat feedback[5000];
	//glGetBufferSubData(GL_TRANSFORM_FEEDBACK_BUFFER, 0, sizeof(feedback), feedback);
	//for (int i = 0; i < 3*6*3; i = i + 3 ) {
	//	std::cout << feedback[i] << ", " <<
	//		feedback[i + 1] << ", " <<
	//		feedback[i + 2] << ", " << std::endl;
	//		if (i % 9 == 0)
	//			std::cout <<  "----------" << std::endl;
	//}
	
	glBindVertexArray(0);

}
