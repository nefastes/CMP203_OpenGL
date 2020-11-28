#pragma once
#include "glut.h"
#include <gl/gl.h>
#include <gl/glu.h>
#include <math.h>
#include <vector>
#define PI 3.1415f
class BasicShape
{
protected:
	//Color of the shape
	float red = 1.f, green = 1.f, blue = 1.f;
	//A trasparency tracker, to be true if we want a transparent shape
	bool transparent = false;
	float alpha = 1.f;
	//The shape's texture, to stay NULL if none is wanted
	GLuint* texture = nullptr;
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

	void setTransparent(bool t) { transparent = t; }
	void setTransparency(float a) { alpha = a; transparent = true; }
	bool isTranspearent() { return transparent; }

	void setColor3f(float r, float g, float b) { red = r; green = g; blue = b; }
	void setColor4f(float r, float g, float b, float a) { red = r; green = g; blue = b; alpha = a; transparent = true; }

	void setTexture(GLuint& tex) { texture = &tex; }
	GLuint* getTexture() { return texture; }
};

