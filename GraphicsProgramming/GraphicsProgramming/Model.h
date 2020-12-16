// Model class, represents a loaded OBJ model
// handles the loading/processing of the model file. Also loads and stores its own texture
// NOT complete, for students to complete. Model data requires being sorted and the render function needs completed.
#ifndef _MODEL_H_
#define _MODEL_H_

// INCLUDES //
#include <glut.h>
#include <fstream>
#include <gl/gl.h>
#include <gl/glu.h>

using namespace std;

// MY CLASS INCLUDES //
#include <vector>
#include <array>
#include "Vector3.h"
#include "SOIL.h"

class Model
{

public:

	bool load(char* modelFilename, char* textureFilename);
	void render(short unsigned textureFilter);
	void setAsLight(bool l) { lightSource = l; }

private:

	void loadTexture(char*);
	bool loadModel(char*);

	// model texture
	GLuint texture;

	// Stoagre for sorted data
	vector<float> vertex, normals, texCoords;

	//Bool to check whether the model is made of quads
	unsigned numberTrianglesToRender = 0;
	unsigned numberQuadsToRender = 0;

	//Bool to track whether the model is intented to appear as a light source
	bool lightSource = false;
};

#endif