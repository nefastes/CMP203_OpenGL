#pragma once
#include "glut.h"
#include <gl/gl.h>
#include <gl/glu.h>
#include <math.h>
#include <vector>
#define PI 3.1415f
class BasicShape
{
private:
	//A basic shape object will render a cube 

protected:
	//Vectors used for procedural generation
	std::vector<float> vertices;
	std::vector<float> normals;
	std::vector<float> texCoordinates;
	std::vector<GLubyte> indices;

public:
	BasicShape();
	~BasicShape();

	virtual void render();
	virtual void generateShape();
};

