#pragma once

namespace toyData {

	//
	// Properties 
	//

	const float cubeVertexColorUVs[] = {
		// positions			// colors		// UVs
		0.2f,  0.2f, 0.3f,  1.0f, 0.0f, 0.0f,  2.0f, 2.0f, // top right
		0.2f, -0.2f, 0.3f,  1.0f, 1.0f, 0.0f,  2.0f, 0.0f, // bottom right
		-0.2f, -0.2f, 0.3f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f, // bottom left
		-0.2f,  0.2f, 0.3f,  1.0f, 0.0f, 1.0f,  0.0f, 2.0f, // top left 

		// Backvertices
		0.2f,  0.2f, -0.3f,  1.0f, 0.0f, 0.0f,  2.0f, 2.0f, // top right
		0.2f, -0.2f, -0.3f,  1.0f, 1.0f, 0.0f,  2.0f, 0.0f, // bottom right
		-0.2f, -0.2f, -0.3f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f, // bottom left
		-0.2f,  0.2f, -0.3f,  1.0f, 0.0f, 1.0f,  0.0f, 2.0f, // top left 

	};
	const unsigned int cubeIndices[] = {  // note that we start from 0!
		0, 1, 3,  // first Triangle
		1, 2, 3,   // second Triangle

		4, 5, 7,  // back first Triangle
		5, 6, 7,   // back  second Triangle

		4, 0, 7,  // top first Triangle
		0, 3, 7,   // top  second Triangle

		5, 1, 6,  // bottom first Triangle
		1, 2, 6,   // bottom second Triangle

		4, 5, 0,  // right first Triangle
		5, 1, 0,  // right second Triangle

		3, 2, 7,  // left first Triangle
		2, 6, 7   // left second Triangle
	};

	const float cubeVerticesOnly[] = {
		// positions
		0.2f,  0.2f, 0.3f,   // top right
		0.2f, -0.2f, 0.3f,   // bottom right
		-0.2f, -0.2f, 0.3f,  // bottom left
		-0.2f,  0.2f, 0.3f,  // top left 

		// Backvertices
		0.2f,  0.2f, -0.3f,   // top right
		0.2f, -0.2f, -0.3f,   // bottom right
		-0.2f, -0.2f, -0.3f,  // bottom left
		-0.2f,  0.2f, -0.3f,  // top left 

	};

}