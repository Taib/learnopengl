#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<iostream>


#include"Shader.h"
#include"Mesh.h"
#include"Texture.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main()
{
	// Properties 
	int windowHeight = 600, windowWidth = 800;
	float vertices[] = {
		// positions			// colors		// UVs
		 0.85f,  0.85f, 0.0f,  1.0f, 0.0f, 0.0f,  2.0f, 2.0f, // top right
		 0.85f, -0.85f, 0.0f,  1.0f, 1.0f, 0.0f,  2.0f, 0.0f, // bottom right
		-0.85f, -0.85f, 0.0f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f, // bottom left
		-0.85f,  0.85f, 0.0f,  1.0f, 0.0f, 1.0f,  0.0f, 2.0f, // top left 
	};
	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,  // first Triangle
		1, 2, 3   // second Triangle
	};
	const char* vertextShaderPath = "Shaders/baseVert.vs";
	const char* fragmentShaderPath = "Shaders/baseAnimFrag.fs";

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
	

	// Setting up a callback for window resizing
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);



	// INIT MODEL And Shaders
	Mesh* mesh = new  Mesh(vertices, indices, sizeof(vertices)/sizeof(vertices[0]), sizeof(indices) / sizeof(indices[0])); 
	Shader* shader =  new Shader(vertextShaderPath, fragmentShaderPath);
	Texture* cartoonTex = new Texture("Resources/cartoon.png", 0, false);
	Texture* checkerBoardTex = new Texture("Resources/diffuse_puzzle.png", 1, false);

	// uncomment this call to draw in wireframe polygons.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	// Rendering loop
	while (!glfwWindowShouldClose(window))
	{
		// input
		processInput(window);

		// rendering commands here
		glClearColor(0.2f, 0.2f, 0.3f, 0.1f); // We want to clear the screen with a color of our choice. 
		glClear(GL_COLOR_BUFFER_BIT); // The possible bits we can set are GL_COLOR_BUFFER_BIT, GL_DEPTH_BUFFER_BIT and GL_STENCIL_BUFFER_BIT. 


		// Bind the shader
		shader->use();

		//cartoonTex->bind(); // use a texture
		//checkerBoardTex->bind(); // use a texture


		float timeValue = glfwGetTime(); // we retrieve the running time in seconds  
		shader->setFloat("time", glfwGetTime());

		shader->setInt("mainTexture", 0);
		shader->setInt("secondTexture", 1);

		// Draw the model
		mesh->draw();

		// check and call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
 

	// Properly clean/delete all of GLFW's resources that were allocated.
	glfwTerminate();

	return 0;

}




void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}


void processInput(GLFWwindow* window)
{
	// Here we check whether the user has pressed the escape key (if it's not pressed, glfwGetKey returns GLFW_RELEASE)
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}