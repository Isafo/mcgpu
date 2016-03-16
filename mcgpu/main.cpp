#include "GLew\glew.h"

#include "glfwContext.h"
#include "Shader.h"
#include "MatrixStack.h"
#include "Sphere.h"
#include "Camera.h"
#include "DynamicMesh.h"
#include "Octant.h"
#include "testGenerator.h"

#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> //glm::make:mat4
#include <glm\gtx\rotate_vector.hpp> // rotate vector

#include <iostream>

void inputHandler(GLFWwindow* _window, double _dT);
void cameraHandler(GLFWwindow* _window, double _dT, Camera* _cam);
void GLcalls();

int main(){

	glm::mat4 test = glm::mat4(1);

	glfwContext glfw;
	GLFWwindow* currentWindow = nullptr;

	//glfw.init(640, 480, "mCity");
	glfw.init(1920, 1080, "mCity");
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

	//scene objects
	Sphere testSphere(0.0f, 0.0f, -1.0f, 0.1f);

	Sphere lightOne(0.5f, 0.5f, 1.0f, 0.1f);
	//TODO: do this properly
	glm::vec4 LP = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);// glm::vec4(lightOne.getPosition()[0], lightOne.getPosition()[1], lightOne.getPosition()[2], 1.0f);
	glm::mat4 lightT = glm::mat4(1.0f);

	Octant Octree;
	testGenerator firstTest;
	DynamicMesh firstMesh;
	firstMesh.createBuffers();
	firstMesh.genTableTex();
	firstTest.generate(&Octree, &firstMesh);

	Camera mCamera;
	mCamera.setPosition(&glm::vec3(0.0f, 0.0f, -4.0f));


	double lastTime = glfwGetTime() - 0.001;
	double dT = 0.0;
	while (!glfwWindowShouldClose(currentWindow))
	{
		dT = glfwGetTime() - lastTime;
		lastTime = glfwGetTime();

		//glfw input handler
		inputHandler(currentWindow, dT);
		//cameraHandler(currentWindow, dT, &mCamera);

		GLcalls();

		glUseProgram(sceneLight.programID);
		
		
		MVstack.push();//Camera transforms --<
			glUniformMatrix4fv(locationP, 1, GL_FALSE, mCamera.getPerspective());

			//glm::transpose(cameraT);
			MVstack.multiply(mCamera.getTransformM()); 


			glUniform3fv(locationLP, 1, &(*(MVstack.getCurrentMatrixM())*glm::vec4(*(lightOne.getPositionV()), 1.0f))[0]);
			MVstack.push();//light transforms --<
				//MVstack.translate(lightOne.getPositionV());
				MVstack.multiply(lightOne.getTransformM());
				glUniformMatrix4fv(locationMV, 1, GL_FALSE, MVstack.getCurrentMatrix());
				lightT = glm::make_mat4(MVstack.getCurrentMatrix());
				//glm::transpose(lightT);
				//glUniform3fv(locationLP, 1, &(lightT*LP)[0]);

				//glBindTexture(GL_TEXTURE_2D, greyTex.getTextureID());
				lightOne.render();
			MVstack.pop(); //light transforms >--

			MVstack.push();//sphere transforms --<
				MVstack.multiply(firstMesh.getOrientation());
				MVstack.translate(firstMesh.getPosition());
				glUniformMatrix4fv(locationMV, 1, GL_FALSE, MVstack.getCurrentMatrix());
				//glBindTexture(GL_TEXTURE_2D, greyTex.getTextureID());
				firstMesh.render();
			MVstack.pop(); //sphere transforms >--
			MVstack.push();//Plane transforms --<
				MVstack.multiply(firstMesh.getOrientation());
				MVstack.translate(firstMesh.getPosition());
				glUniformMatrix4fv(locationMV, 1, GL_FALSE, MVstack.getCurrentMatrix());
				//glBindTexture(GL_TEXTURE_2D, greyTex.getTextureID());
				firstMesh.render();
			MVstack.pop(); //Plane transforms >--

		MVstack.pop(); //Camera transforms >--

		glfwSwapBuffers(currentWindow);
		glfwPollEvents();
	}
	
	return 0;
}

void inputHandler(GLFWwindow* _window, double _dT)
{
	if (glfwGetKey(_window, GLFW_KEY_ESCAPE)) {
		glfwSetWindowShouldClose(_window, GL_TRUE);
	}
	
}
void cameraHandler(GLFWwindow* _window, double _dT, Camera* _cam)
{
	glm::vec3 translation;
	glm::mat4* T = _cam->getTransformM();
	glm::vec3* cDirection = _cam->getDirection();
	glm::vec3 cRight = glm::vec3((*T)[0][0], (*T)[1][0], (*T)[2][0]);
	glm::vec3 cUp = glm::vec3((*T)[0][1], (*T)[1][1], (*T)[2][1]);

	if (glfwGetKey(_window, GLFW_KEY_W)) {
		translation = *cDirection*-0.5f*(float)_dT;
		_cam->translate(&translation);
	}
	if (glfwGetKey(_window, GLFW_KEY_S)){
		translation = *cDirection*0.5f*(float)_dT;
		_cam->translate(&translation);
	}
	if (glfwGetKey(_window, GLFW_KEY_A)) {
		translation = cRight*0.5f*(float)_dT;
		_cam->translate(&translation);
	}
	if (glfwGetKey(_window, GLFW_KEY_D)) {
		translation = cRight*-0.5f*(float)_dT;
		_cam->translate(&translation);
	}

	double X, Y, dX, dY;
	glfwGetCursorPos(_window, &X, &Y);

	dX = (X - _cam->mouse.x) / 640.0;
	dY = (Y - _cam->mouse.y) / 480.0;

	_cam->mouse.x = X;
	_cam->mouse.y = Y;

	
	//*cDirection = glm::rotate(*cDirection, (float)dY, cRight);
	*cDirection = glm::rotateX(*cDirection, (float)dY); *cDirection = glm::rotateY(*cDirection, (float)dX);
	//*cRight = glm::rotate(*cRight, (float)-dY, cUp);
	//*cDirection = glm::rotate(*cDirection, (float)dX, cUp);

	_cam->updateRot();

	//glm::rotate(*cDirection, (float)X, cUp);
	//glm::rotate(*cDirection, (float)Y, *cRight);


}

void GLcalls()
{
	glClearColor(0.01f, 0.01f, 0.01f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	//glEnable(GL_FRAMEBUFFER_SRGB);
	//glEnable(GL_FLAT);
	//glShadeModel(GL_FLAT);
	glCullFace(GL_BACK);
	//glDisable(GL_TEXTURE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glFrontFace(GL_CCW);
}
