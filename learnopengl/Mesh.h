#pragma once
#include <glad/glad.h>
class Mesh {
public:
	Mesh();
	~Mesh();
	void draw();

	// Create a Mesh with Vertices, Color, and Texture coordinates provided
	void CreateVCT(const float* vertices, const unsigned int* indices, unsigned int numVertices, unsigned int numIndices);

	// Create a Mesh with only Vertices provided
	void CreateV(const float* vertices, const unsigned int* indices, unsigned int numVertices, unsigned int numIndices);


private:
	unsigned int VBO;
	unsigned int VAO;
	unsigned int EBO;
	unsigned int indicesCount;
};


Mesh::Mesh( )
{
	VBO = 0;
	VAO = 0;
	EBO = 0;
	indicesCount = -1;
}

void Mesh::CreateVCT(const float* vertices, const unsigned int* indices, unsigned int numVertices, unsigned int numIndices)
{
	indicesCount = numIndices;
	//  any subsequent vertex attribute calls from that point on will be stored inside the VAO. 
	glGenVertexArrays(1, &VAO);

	// bind Vertex Array Object
	glBindVertexArray(VAO);

	// Creating and binding the Element Buffer Object
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * numIndices, indices, GL_STATIC_DRAW);


	glGenBuffers(1, &VBO); // Generate a buffer ID
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // Bind the buffer to the current VAO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * numVertices, vertices, GL_STATIC_DRAW); // Send to the GPU 

	// we can tell OpenGL how it should interpret the vertex data (per vertex attribute) using glVertexAttribPointer
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0); // giving the vertex attribute location as its argument; As given in the Vertex Shade
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

}

void Mesh::CreateV(const float* vertices, const unsigned int* indices, unsigned int numVertices, unsigned int numIndices)
{
	indicesCount = numIndices;
	//  any subsequent vertex attribute calls from that point on will be stored inside the VAO. 
	glGenVertexArrays(1, &VAO);

	// bind Vertex Array Object
	glBindVertexArray(VAO);

	// Creating and binding the Element Buffer Object
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * numIndices, indices, GL_STATIC_DRAW);


	glGenBuffers(1, &VBO); // Generate a buffer ID
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // Bind the buffer to the current VAO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * numVertices, vertices, GL_STATIC_DRAW); // Send to the GPU 

	// we can tell OpenGL how it should interpret the vertex data (per vertex attribute) using glVertexAttribPointer
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0); // giving the vertex attribute location as its argument; As given in the Vertex Shade

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

}
Mesh::~Mesh()
{
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);
}

void Mesh::draw()
{
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glDrawArrays(GL_TRIANGLES, 0, 3); // the starting index of the vertex array we'd like to draw, and how many vertices  
	glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, 0);
	//glBindVertexArray(0); // no need to unbind it every time
}