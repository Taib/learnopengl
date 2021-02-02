#ifndef TEXTURE_FILE
#define TEXTURE_FILE

#include<iostream>
#include<glad/glad.h>

// By defining STB_IMAGE_IMPLEMENTATION the preprocessor modifies the header file
// such that it only contains the relevant definition source code, 
// effectively turning the header file into a .cpp file
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"



class Texture
{
public:
	Texture(const char* imagePath, int index, bool hasAlpha = true);
	~Texture();

	// bind the texture
	void bind();

private:
	unsigned int texture;
	int index;
};

#endif // !TEXTURE_FILE

Texture::Texture(const char* imagePath, int index=0, bool hasAlpha)
{	
	texture = 0; // init
	this->index = index;
	
	float borderColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };

	// load the texture data
	int width, height, nChannels;
	stbi_set_flip_vertically_on_load(true); // flip to be comform with OpenGL standard
	unsigned char* data = stbi_load(imagePath, &width, &height, &nChannels, 0);
	if (!data)
	{
		std::cout << "ERROR::TEXTURE:: Failed to load texture" << std::endl;
		throw "Image loading error";
	}

	// generate texture object ID
	glGenTextures(1, &texture);

	// bind the texture the current context
	glActiveTexture(GL_TEXTURE0+index);
	// bind the texture the current context
	glBindTexture(GL_TEXTURE_2D, texture);

	// set the texture wrapping options on the currently bound texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	// set the texture filtering options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// send the data to the GPU
	if(hasAlpha)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	else
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	// generate mipmaps automatically
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data); // never forget to free the memory

}

Texture::~Texture()
{
	if (texture != 0)
		glDeleteTextures(1, &texture);
}


 
void Texture::bind()
{ 
	glActiveTexture(GL_TEXTURE0 + this->index);
	glBindTexture(GL_TEXTURE_2D, texture); 
}