#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<iostream>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

class Model {
public: 
	Model(const float* vertices, const unsigned int * indices, unsigned int numVertices, unsigned int numIndices) 
	{
		indicesCount = numIndices;
		//  any subsequent vertex attribute calls from that point on will be stored inside the VAO. 
		glGenVertexArrays(1, &VAO);

		// 1. bind Vertex Array Object
		glBindVertexArray(VAO);

		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * numIndices, indices, GL_STATIC_DRAW);


		glGenBuffers(1, &VBO); // Generate a buffer ID
		glBindBuffer(GL_ARRAY_BUFFER, VBO); // Bind the buffer to the current VAO
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * numVertices, vertices, GL_STATIC_DRAW); // Send to the GPU


		// we can tell OpenGL how it should interpret the vertex data (per vertex attribute) using glVertexAttribPointer
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0); // giving the vertex attribute location as its argument; As given in the Vertex Shade
		
		// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
		glBindBuffer(GL_ARRAY_BUFFER, 0); 

		// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
		// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
		glBindVertexArray(0); 
 
	}
	~Model() 
	{
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
		glDeleteVertexArrays(1, &VAO);
	}

	void draw()
	{
		glBindVertexArray(VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		//glDrawArrays(GL_TRIANGLES, 0, 3); // the starting index of the vertex array we'd like to draw, and how many vertices  
		glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, 0);
		//glBindVertexArray(0); // no need to unbind it every time
	}
private:
	unsigned int VBO;
	unsigned int VAO;
	unsigned int EBO;
	unsigned int indicesCount;
};

class Shader {
public:
	Shader(const char* vertShaderText, const char* fragShaderText)
	{ 
		bindShader(vertShaderText, GL_VERTEX_SHADER);
		bindShader(fragShaderText, GL_FRAGMENT_SHADER);

		// A shader program object is the final linked version of multiple shaders combined. 
		// To use the recently compiled shaders we have to link them to a shader program object
		// and then activate this shader program when rendering objects. 
		shaderProgram = glCreateProgram(); //  returns the ID reference to the newly created program object.

		// We need to attach the previously compiled shaders to the program object and then link them with glLinkProgram: 
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);

		// We can also check if linking a shader program failed
		int success;
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (!success)
		{
			char infoLog[512];
			glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM_LINKING_FAILED \n" << infoLog << std::endl;

		}

		// we no longer need them anymore
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}
	~Shader()
	{
		glDeleteProgram(shaderProgram);
	}
	int getUniformLocation(const char* name)
	{
		return glGetUniformLocation(shaderProgram, name);
	}
	void use() 
	{
		glUseProgram(shaderProgram);
	}

private:
	void bindShader(const char* source, GLenum type)
	{
		unsigned int* shader = &vertexShader;
		if (type == GL_FRAGMENT_SHADER)
			shader = &fragmentShader; 


		*shader = glCreateShader(type);
		glShaderSource(*shader, 1, &source, NULL);
		glCompileShader(*shader);

		// Checking for compile - time errors is accomplished as follows :
		int success;
		glGetShaderiv(*shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			char infoLog[512];
			glGetShaderInfoLog(*shader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::COMPILATION_FAILED \n" << infoLog << std::endl;
		}
	}
	 
	unsigned int vertexShader;
	unsigned int fragmentShader;
	unsigned int shaderProgram;
};

int main()
{
	// Properties 
	int windowHeight = 600, windowWidth = 800;
	float vertices[] = {
		 0.5f,  0.5f, 0.0f,  // top right
		 0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f   // top left 
	};
	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,  // first Triangle
		1, 2, 3   // second Triangle
	};
	const char* vertextShaderText = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"out vec4 vertexColor;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"   vertexColor = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";
	const char* fragmentShaderText = "#version 330 core\n"
		"out vec4 FragColor;\n" 
		"in vec4 vertexColor;\n" 
		"uniform vec4 ourColor;\n" 
		"void main()\n"
		"{\n"
		"	FragColor = ourColor*vertexColor;\n"
		"}\n";

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
	Model* model = new  Model(vertices, indices, sizeof(vertices)/sizeof(vertices[0]), sizeof(indices) / sizeof(indices[0])); 
	Shader* shader =  new Shader(vertextShaderText, fragmentShaderText);

	int ourColorLocation = shader->getUniformLocation("ourColor");//  We first need to find the index/location of the uniform attribute in our shader.


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


		float timeValue = glfwGetTime(); // we retrieve the running time in seconds 
		float gValue = (sin(timeValue) / 2.0) + 0.5f; // we vary the color in the range of [0.0 - 1.0] 
		glUniform4f(ourColorLocation, gValue, 1, 1, 1);

		// Draw the model
		model->draw();

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