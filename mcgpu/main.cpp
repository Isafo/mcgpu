#include "GLew\glew.h"

#include "glfwContext.h"
#include "Shader.h"
#include "MatrixStack.h"
#include "Sphere.h"
#include "Camera.h"

#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> //glm::make:mat4

#include <iostream>

int main(){

	glm::mat4 test = glm::mat4(1);

	glfwContext glfw;
	GLFWwindow* currentWindow = nullptr;

	glfw.init(640, 480, "mCity");
	glfw.getCurrentWindow(currentWindow);

	//start GLEW extension handler
	glewExperimental = GL_TRUE;
	GLenum l_GlewResult = glewInit();
	if (l_GlewResult != GLEW_OK) 
		std::cout << "glewInit() error." << std::endl;
	
	// Print some info about the OpenGL context...
	glfw.printGLInfo();

	Shader sceneLight;
	sceneLight.createShader("shaders/sceneV.glsl", "shaders/sceneF.glsl");

	GLint locationP = glGetUniformLocation(sceneLight.programID, "P"); //perspective matrix
	GLint locationMV = glGetUniformLocation(sceneLight.programID, "MV"); //modelview matrix
	GLint locationM = glGetUniformLocation(sceneLight.programID, "M"); //modelview matrix
	GLint locationLP = glGetUniformLocation(sceneLight.programID, "LP"); //modelview matrix
	//	GLint* MVptr = &locationMV;
	GLint locationTex = glGetUniformLocation(sceneLight.programID, "tex"); //texcoords

	MatrixStack MVstack; MVstack.init();

	Sphere testSphere(0.5f, 0.0f, -0.5f, 0.1f);
	Sphere lightOne(0.5f, 0.5f, -2.0f, 0.1f);
	//TODO: do this properly
	glm::vec4 LP = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);// glm::vec4(lightOne.getPosition()[0], lightOne.getPosition()[1], lightOne.getPosition()[2], 1.0f);
	glm::mat4 lightT = glm::mat4(1.0f);
	
	Camera mCamera;
	mCamera.setPosition(&glm::vec3(0.0f, 0.0f, -1.0f));

	while (!glfwWindowShouldClose(currentWindow))
	{

		if (glfwGetKey(currentWindow, GLFW_KEY_ESCAPE)) {
			glfwSetWindowShouldClose(currentWindow, GL_TRUE);
		}

		glUseProgram(sceneLight.programID);
		
		
		MVstack.push();//Camera transforms --<
			glUniformMatrix4fv(locationP, 1, GL_FALSE, mCamera.getPerspective());

			//glm::transpose(cameraT);
			MVstack.multiply(mCamera.getTransformM());

			MVstack.push();//light transforms --<
				//MVstack.translate(lightOne.getPositionV());
				MVstack.multiply(lightOne.getTransformM());
				glUniformMatrix4fv(locationMV, 1, GL_FALSE, MVstack.getCurrentMatrix());
				lightT = glm::make_mat4(MVstack.getCurrentMatrix());
				//glm::transpose(lightT);
				glUniform3fv(locationLP, 1, &(lightT*LP)[0]);

				//glBindTexture(GL_TEXTURE_2D, greyTex.getTextureID());
				lightOne.render();
			MVstack.pop(); //light transforms >--

			MVstack.push();//sphere transforms --<
				MVstack.multiply(testSphere.getTransformM());
			//	MVstack.translate(testSphere.getPositionV());
				glUniformMatrix4fv(locationMV, 1, GL_FALSE, MVstack.getCurrentMatrix());
				//glBindTexture(GL_TEXTURE_2D, greyTex.getTextureID());
				testSphere.render();
			MVstack.pop(); //sphere transforms >--

		MVstack.pop(); //Camera transforms >--

		glfwSwapBuffers(currentWindow);
		glfwPollEvents();
	}
	
	return 0;
}