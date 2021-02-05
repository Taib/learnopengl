
#include<iostream>

#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr


#include"Shader.h"
#include"Mesh.h"
#include"Texture.h"
#include"Camera.h"

#include"ToyMeshData.h"

//
// Callback functions definition
//

int main();

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouseInput(GLFWwindow* window, double mouseXPos, double mouseYPos);
void scrollInput(GLFWwindow* window, double xOffset, double yOffset);

//
// Global Variables
//

int windowHeight = 600, windowWidth = 800; // screen resolution

// Mouse events
float lastMouseY = windowHeight / 2.0f, lastMouseX = windowWidth / 2.0f; // last recorded mouse position
float firstMouse = true; // to avoid mouse jumping in the initial frame

// Timing
float deltaTime = 0.0f; // Time between current frame and last frame
float lastFrameTime = 0.0f; // Time of last frame

// Camera model
Camera* camera = new Camera();

//
// Main function
//
int main()
{
	// we first initialize GLFW, after which we can configure GLFW using glfwWindowHint
	glfwInit();

	// The first argument of glfwWindowHint tells us what option we want to configure
	// The second argument is an integer that sets the value of our option
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // we'd like to tell GLFW that 3.3 is the OpenGL
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // we want to explicitly use the core-profile.

	// Note that on Mac OS X you need to add glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); to your initialization code for it to work. 
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create a window object that holds all the windowing data 
	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "LearnOpenGL", NULL, NULL);
	if (window == NULL) 
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	// We mentioned that GLAD manages function pointers for OpenGL 
	// so we want to initialize GLAD before we call any OpenGL function
	
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		// We pass GLAD the function to load the address of the OpenGL function pointers which is OS-specific. 
		// GLFW gives us glfwGetProcAddress that defines the correct function based on which OS we're compiling for.

		std::cout << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
		return -1;
	}

	// We have to tell OpenGL the size of the rendering window 
	// so OpenGL knows how we want to display the data and coordinates with respect to the window. 
	glViewport(0, 0, windowWidth, windowHeight); // The first two parameters of glViewport set the location of the lower left corner of the window.
	
	//
	// CallBacks
	// ---------
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);// Setting up a callback for window resizing 
	glfwSetCursorPosCallback(window, mouseInput); // callback for mouse movements
	glfwSetScrollCallback(window, scrollInput); // callback for mouse movements

	// INIT MODEL And Shaders
	// ----------------------
	Mesh* cubeMesh = new  Mesh();
	cubeMesh->CreateVCT(
		toyData::cubeVertexColorUVs, 
		toyData::cubeIndices, 
		sizeof(toyData::cubeVertexColorUVs) / sizeof(toyData::cubeVertexColorUVs[0]), 
		sizeof(toyData::cubeIndices) / sizeof(toyData::cubeIndices[0])
	);
	Mesh* lightCube = new  Mesh();
	lightCube->CreateV(
		toyData::cubeVerticesOnly,
		toyData::cubeIndices,
		sizeof(toyData::cubeVerticesOnly) / sizeof(toyData::cubeVerticesOnly[0]),
		sizeof(toyData::cubeIndices) / sizeof(toyData::cubeIndices[0])
	);
	Shader* cubeShader = new Shader("Shaders/Ch1/cameraVert.vs", "Shaders/Ch2/baseLighting.fs");
	Shader* lightShader = new Shader("Shaders/Ch2/lightVert.vs", "Shaders/Ch2/lightFrag.fs");
	Texture* cartoonTex = new Texture("Resources/cartoon.png", 0, false);
	Texture* checkerBoardTex = new Texture("Resources/diffuse_puzzle.png", 1, false);
	 

	// tell GLFW that it should hide the cursor and capture it
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	 
	
	// uncomment this call to draw in wireframe polygons.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	// Enable depth testing to avoid drawing hidden objects in the back
	glEnable(GL_DEPTH_TEST);
	// Rendering loop
	while (!glfwWindowShouldClose(window))
	{

		// per-frame time logic
		float curTime = glfwGetTime();
		deltaTime = curTime - lastFrameTime;
		lastFrameTime = curTime;

		// input
		processInput(window); 

		// rendering commands here
		glClearColor(0.0f, 0.2f, 0.3f, 0.1f); // We want to clear the screen with a color of our choice. 
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // The possible bits we can set are GL_COLOR_BUFFER_BIT, GL_DEPTH_BUFFER_BIT and GL_STENCIL_BUFFER_BIT. 


		glm::mat4 projectionMat = glm::perspective(glm::radians(camera->getFOV()), (float)windowWidth / (float)windowHeight, 0.1f, 100.f);
		
		/// First Mesh 
		// --------------------------------------------------------------------------------------

		// Bind the shader
		cubeShader->use();
		cubeShader->setMat4("projection", projectionMat);

		cartoonTex->bind(); // use a texture
		checkerBoardTex->bind(); // use a texture 
		  
		cubeShader->setFloat("time", curTime);

		cubeShader->setFloat3("objectColor", glm::vec3(0.2f, 0.8f, 0.3f));
		cubeShader->setFloat3("lightColor", glm::vec3(1.0f));

		cubeShader->setMat4("view", camera->getViewMatrix());
		
		glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		float angle = 20.0f ;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		cubeShader->setMat4("model", model); 
		// Draw the model
		cubeMesh->draw();

		/// Second Mesh 
		// --------------------------------------------------------------------------------------
		lightShader->use();

		model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
		model = glm::translate(model, glm::vec3(1.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(45.0f), glm::vec3(1.0f, 0.3f, 0.5f));
		model = glm::scale(model,  glm::vec3(0.3f));

		lightShader->setMat4("PVM", projectionMat * camera->getViewMatrix() * model); 
		
		// Draw the model2
		lightCube->draw();

		// --------------------------------------------------------------------------------------
		// check and call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
 

	// Properly clean/delete all of GLFW's resources that were allocated.
	glfwTerminate();

	return 0;

}



// Window resizing event callback
void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}


// Keyboard events callback
void processInput(GLFWwindow* window)
{
	// Here we check whether the user has pressed the escape key (if it's not pressed, glfwGetKey returns GLFW_RELEASE)
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	} 
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{ 
		camera->keyboardMovement(CAM_FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		camera->keyboardMovement(CAM_BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		camera->keyboardMovement(CAM_LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		camera->keyboardMovement(CAM_RIGHT, deltaTime);
	}

}


// Mouse movement callback
void mouseInput(GLFWwindow* window, double mouseXPos, double mouseYPos)
{
	// if-statement to avoid camera jumping rapidly 
	if (firstMouse) // initially set to true
	{
		lastMouseX = mouseXPos;
		lastMouseY = mouseYPos;
		firstMouse = false;
	}


	float xOffset = mouseXPos - lastMouseX;
	float yOffset = lastMouseY - mouseYPos; // reversed since y-coordinates range from bottom to top
	
	lastMouseX = mouseXPos;
	lastMouseY = mouseYPos;


	camera->mouseMovement(xOffset * deltaTime, yOffset*deltaTime);

}
// Scroll movement callback
void scrollInput(GLFWwindow* window, double xOffset, double yOffset)
{
	camera->scrollMovement(yOffset);
}