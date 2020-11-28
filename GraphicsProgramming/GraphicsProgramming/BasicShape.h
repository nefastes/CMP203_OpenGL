#pragma once
#include "glut.h"
#include <gl/gl.h>
#include <gl/glu.h>
#include <math.h>
#include <vector>
#include "Vector3.h"
#define PI 3.1415f
class BasicShape
{
protected:
	//Color of the shape
	float red = 1.f, green = 1.f, blue = 1.f;
	//A trasparency tracker, to be true if we want a transparent shape
	bool transparent = false;
	float alpha = 1.f;
	//A simple bool to track if we want to render the inside of the shape (true)
	bool renderInside = false;
	//The shape's texture, to stay NULL if none is wanted
	GLuint* texture = nullptr;
	//Vectors used for procedural generation
	std::vector<float> vertices;
	std::vector<float> normals;
	std::vector<float> texCoordinates;
	std::vector<GLubyte> indices;
	//Origin of the shape (the origin is allways in the middle/center of the shape)
	Vector3 origin;
	//Scale of the shape
	Vector3 scale = Vector3(1.f, 1.f, 1.f);

public:
	BasicShape();
	~BasicShape();

	virtual void render();
	virtual void generateShape();

	void setTransparent(bool t) { transparent = t; }
	void setTransparency(float a) { alpha = a; transparent = true; }
	bool isTranspearent() { return transparent; }

	void setInsideRendering(bool r) { renderInside = r; }
	bool isInsideRendered() { return renderInside; }

	void setColor3f(float r, float g, float b) { red = r; green = g; blue = b; }
	void setColor4f(float r, float g, float b, float a) { red = r; green = g; blue = b; alpha = a; transparent = true; }

	void setTexture(GLuint& tex) { texture = &tex; }
	GLuint* getTexture() { return texture; }

	void setPosition(Vector3 pos) { origin = pos; }	//Sets the position of the shape, must be called BEFORE generating it.
	void setPosition(float x, float y, float z) { origin.x = x; origin.y = y; origin.z = z; }	//Sets the position of the shape, must be called BEFORE generating it.
	Vector3 getPosition() { return origin; }

	void setScale(Vector3 s) { scale = s; }
	void setScale(float sx, float sy, float sz) { scale.x = sx; scale.y = sy; scale.z = sz; }
	Vector3 getScale() { return scale; }
	void rescale(Vector3 s) { scale = s; generateShape(); }	//Need to regenerate the shape when rescaling it
	void rescale(float sx, float sy, float sz) { scale.x = sx; scale.y = sy; scale.z = sz; generateShape(); }	//Need to regenerate the shape when rescaling it
};

