#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include "Shader.h"


using namespace std;

class Ocean {
public:
	unsigned int VAO;
	float oceanSeed;
	int numVertsWide;
	float distBetweenVerts;
	float *oceanV;
	float waveHeight = .1f;
	float waveSpeed = .80f;
	float height;
	float width;

	Ocean(int numVertsWide = 500, float distBetweenVerts = .1, float height = -0.2f) {
		// randomize the ocean seed
		// -----------
		oceanSeed = rand();

		this->numVertsWide = numVertsWide;
		this->distBetweenVerts = distBetweenVerts;
		this->height = height;
		this->width = (numVertsWide * distBetweenVerts) / 6.0f;

		// allocate oceanV to be numVertsWide * numVertsWide and have an x, y, and z float value
		oceanV = (float *)malloc(numVertsWide * numVertsWide * 3 * 6 * sizeof(float));
		// initialize oceanV with its vertex coordinates
		InitializeOceanV();

		// set up the vbo and vao stuff
		InitializeBufferData();
	}
	void Draw(Shader s) {
		// load data into vertex buffers
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, numVertsWide * numVertsWide * 3 * 6 * sizeof(float), oceanV, GL_STATIC_DRAW);
		// draw mesh
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, numVertsWide * numVertsWide);
		glBindVertexArray(0);
	}
private:
	/*  Render data  */
	unsigned int VBO;

	/*  Functions    */
	// initializes all the buffer objects/arrays
	void InitializeBufferData()
	{
		// create buffers/arrays
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);
		// load data into vertex buffers
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, numVertsWide * numVertsWide * 3 * 6 * sizeof(float), oceanV, GL_STATIC_DRAW);

		// set the vertex attribute pointers
		// vertex Positions
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		//glBindVertexArray(0);
	}
	float getWaveVal(float row, float col) {
		// this y value calculation has been moved to the ocean vertex shader for efficiency
		// height is used to move the local y position to the specified value
		return 0;//height + (sin(glfwGetTime() - row * col) * waveHeight);
	}
	void InitializeOceanV() {
		// initialize oceanV with its vertex coordinates
		for (int rowIndex = 0; rowIndex < numVertsWide; rowIndex++) {
			for (int colIndex = 0; colIndex < numVertsWide / 6; colIndex++) {
				int rowOffset = (rowIndex * numVertsWide * 3 * 6);
				int colOffset = (colIndex * 3 * 6);


				float scale = 1.0f;
				float rowPos = distBetweenVerts * rowIndex -((numVertsWide / 12) * distBetweenVerts);
				float colPos = distBetweenVerts * colIndex - ((numVertsWide / 12) * distBetweenVerts);
				float nextRowPos = distBetweenVerts * (rowIndex + 1) - ((numVertsWide / 12) * distBetweenVerts);
				float nextColPos = distBetweenVerts * (colIndex + 1) - ((numVertsWide / 12) * distBetweenVerts);
				rowPos = glm::abs(rowPos) * rowPos * scale;
				colPos = glm::abs(colPos) * colPos *  scale;
				nextRowPos = glm::abs(nextRowPos) * nextRowPos * scale;
				nextColPos = glm::abs(nextColPos) * nextColPos * scale;
				//*
				// top left
				oceanV[rowOffset + colOffset] = (rowPos);			// x
				oceanV[rowOffset + colOffset + 1] = height;			// y
				oceanV[rowOffset + colOffset + 2] = (colPos);		// z
																	// top right
				oceanV[rowOffset + colOffset + 3] = (rowPos);		// x
				oceanV[rowOffset + colOffset + 4] = height;			// y
				oceanV[rowOffset + colOffset + 5] = (nextColPos);	// z
																	// bot right
				oceanV[rowOffset + colOffset + 6] = (nextRowPos);	// x
				oceanV[rowOffset + colOffset + 7] = height;			// y
				oceanV[rowOffset + colOffset + 8] = (nextColPos);	// z
																	// top left																
				oceanV[rowOffset + colOffset + 9] = (rowPos);		// x
				oceanV[rowOffset + colOffset + 10] = height;		// y
				oceanV[rowOffset + colOffset + 11] = (colPos);		// z
																	// bottom left
				oceanV[rowOffset + colOffset + 12] = (nextRowPos);	// x
				oceanV[rowOffset + colOffset + 13] = height;		// y
				oceanV[rowOffset + colOffset + 14] = (colPos);		// z
																	// bot right
				oceanV[rowOffset + colOffset + 15] = (nextRowPos);	// x
				oceanV[rowOffset + colOffset + 16] = height;		// y
				oceanV[rowOffset + colOffset + 17] = (nextColPos);	// z
				//*/
				/*
				// top left
				oceanV[rowOffset + colOffset] = (rowIndex)* distBetweenVerts;				// x
				oceanV[rowOffset + colOffset + 1] = 0;			// y
				oceanV[rowOffset + colOffset + 2] = (colIndex)* distBetweenVerts;			// z
																							// top right
				oceanV[rowOffset + colOffset + 3] = (rowIndex)* distBetweenVerts;			// x
				oceanV[rowOffset + colOffset + 4] =0;		// y
				oceanV[rowOffset + colOffset + 5] = (colIndex + 1) * distBetweenVerts;		// z
																							// bot right
				oceanV[rowOffset + colOffset + 6] = (rowIndex + 1) * distBetweenVerts;		// x
				oceanV[rowOffset + colOffset + 7] = 0;	// y
				oceanV[rowOffset + colOffset + 8] = (colIndex + 1) * distBetweenVerts;		// z
																							// top left																
				oceanV[rowOffset + colOffset + 9] = (rowIndex)* distBetweenVerts;			// x
				oceanV[rowOffset + colOffset + 10] = 0;		// y
				oceanV[rowOffset + colOffset + 11] = (colIndex)* distBetweenVerts;			// z
																							// bottom left
				oceanV[rowOffset + colOffset + 12] = ((rowIndex + 1)) * distBetweenVerts;	// x
				oceanV[rowOffset + colOffset + 13] = 0;	// y
				oceanV[rowOffset + colOffset + 14] = (colIndex)* distBetweenVerts;			// z
																							// bot right
				oceanV[rowOffset + colOffset + 15] = ((rowIndex + 1)) * distBetweenVerts;	// x
				oceanV[rowOffset + colOffset + 16] = 0;// y
				oceanV[rowOffset + colOffset + 17] = ((colIndex + 1)) * distBetweenVerts;	// z
				*/
			}
		}
	}
};