#pragma once

#include <glad/glad.h>
#include<iostream>

#include "IOFile.h"

class Shader {
public: 
	// constructor reads and builds the shader
	Shader(const char* vertexPath, const char* fragmentPath); 
	~Shader();
	// Retrieve a uniform location within the shader
	int getUniformLocation(const char* name) const;
	// use/activate the shader
	void use();
	// utility uniform functions
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;

private:
	void bindShader(const char* source, GLenum type);

	unsigned int vertexShader;
	unsigned int fragmentShader;
	unsigned int shaderProgram;
};



Shader::Shader(const char* vertexPath, const char* fragmentPath)
{ 
	bindShader(vertexPath, GL_VERTEX_SHADER);
	bindShader(fragmentPath, GL_FRAGMENT_SHADER);

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
		char infoLog[1024];
		glGetProgramInfoLog(shaderProgram, 1024, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM_LINKING_FAILED \n" << infoLog << std::endl;

	}

	// we no longer need them anymore
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

Shader::~Shader()
{
	glDeleteProgram(shaderProgram);
}

int Shader::getUniformLocation(const char* name) const
{
	return glGetUniformLocation(shaderProgram, name);
} 

void Shader::use()
{
	glUseProgram(shaderProgram);
}

void Shader::setBool(const std::string& name, bool value) const
{ 
	glUniform1i(getUniformLocation(name.c_str()), value);
}
void Shader::setInt(const std::string& name, int value) const
{
	glUniform1i(getUniformLocation(name.c_str()), value);
}
void Shader::setFloat(const std::string& name, float value) const
{
	glUniform1f(getUniformLocation(name.c_str()), value);
}


void Shader::bindShader(const char* sourcePath, GLenum type)
{

	std::string source = IOFile::readFile(sourcePath); // tried to return directly a const char* but exception thrown
	const char* sourceText = source.c_str();           // so using first a string and converting it to a c_str

	unsigned int* shader = &vertexShader;

	if (type == GL_FRAGMENT_SHADER)
		shader = &fragmentShader;


	*shader = glCreateShader(type);
	glShaderSource(*shader, 1, &sourceText, NULL);
	glCompileShader(*shader);

	// Checking for compile - time errors is accomplished as follows :
	int success;
	glGetShaderiv(*shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char infoLog[1024];
		glGetShaderInfoLog(*shader, 1024, NULL, infoLog);
		std::cout << "ERROR::SHADER::" << ((type == GL_FRAGMENT_SHADER) ? "FRAGMENT" : "VERTEX") << "::COMPILATION_FAILED \n" << infoLog << std::endl;
	}
}

 